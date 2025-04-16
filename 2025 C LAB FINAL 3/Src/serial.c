#include "serial.h"
#include "stm32f303xc.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

static uint8_t  rx_buffer[BUFFER_SIZE];
static uint16_t rx_index = 0;
static char     term_char = '\r';


struct _SerialPort {
    USART_TypeDef *UART;
    GPIO_TypeDef  *GPIO;
    volatile uint32_t MaskAPB2ENR;
    volatile uint32_t MaskAPB1ENR;
    volatile uint32_t MaskAHBENR;
    volatile uint32_t SerialPinModeValue;
    volatile uint32_t SerialPinSpeedValue;
    volatile uint32_t SerialPinAlternatePinValueLow;
    volatile uint32_t SerialPinAlternatePinValueHigh;
    void (*completion_function)(char *, uint32_t);
};


SerialPort USART1_PORT = {
    USART1,
    GPIOC,
    RCC_APB2ENR_USART1EN,
    0x00,
    RCC_AHBENR_GPIOCEN,
    0xA00,
    0xF00,
    0x770000,
    0x00,
    0x00
};


static void handle_rx_char(char c, SerialPort *serial_port)
{

    SerialOutputChar((uint8_t)c, serial_port);


    if (rx_index < BUFFER_SIZE - 1) {
        rx_buffer[rx_index++] = (uint8_t)c;
    } else {
        SerialOutputString((uint8_t *)"[ERROR] Buffer Overflow!\r\n", serial_port);
        rx_index = 0;
        return;
    }


    if (c == term_char) {
        rx_buffer[rx_index - 1] = '\0';
        SerialOutputString(rx_buffer, serial_port);
        SerialOutputString((uint8_t *)"\r\ndone...\r\n", serial_port);

        if (serial_port->completion_function)
            serial_port->completion_function((char *)rx_buffer, rx_index - 1);

        rx_index = 0;
    }
}


void SerialOutputChar(uint8_t data, SerialPort *serial_port)
{
    while (!(serial_port->UART->ISR & USART_ISR_TXE)) {}
    serial_port->UART->TDR = data;
}

void SerialOutputString(uint8_t *pt, SerialPort *serial_port)
{
    while (*pt) SerialOutputChar(*pt++, serial_port);
}


void SerialPrintPrompt(SerialPort *serial_port)
{
    static int shown = 0;
    if (!shown) {
        SerialOutputString((uint8_t *)"User input interface (end with '#'):\r\n", serial_port);
        shown = 1;
    }
}



void SerialPrintLastLength(SerialPort *serial_port)
{
    extern volatile uint16_t last_received_length;
    static uint32_t tick = 0;
    if (++tick >= 800000) {
        tick = 0;
        char buf[64];
        sprintf(buf, "Last length: %u\r\n", last_received_length);
        SerialOutputString((uint8_t *)buf, serial_port);
    }
}


void SerialInitialise(uint32_t baudRate,
                      SerialPort *serial_port,
                      void (*completion_function)(char *, uint32_t))
{
    serial_port->completion_function = completion_function;


    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHBENR  |= serial_port->MaskAHBENR;

    serial_port->GPIO->MODER   = serial_port->SerialPinModeValue;
    serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;
    serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
    serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

    RCC->APB1ENR |= serial_port->MaskAPB1ENR;
    RCC->APB2ENR |= serial_port->MaskAPB2ENR;


    uint16_t *brr = (uint16_t *)&serial_port->UART->BRR;
    switch (baudRate) {
    case BAUD_9600:   *brr = 8000000 / 9600;   break;
    case BAUD_19200:  *brr = 8000000 / 19200;  break;
    case BAUD_38400:  *brr = 8000000 / 38400;  break;
    case BAUD_57600:  *brr = 8000000 / 57600;  break;
    case BAUD_115200: *brr = 8000000 / 115200; break;
    default:          *brr = 8000000 / 9600;   break;
    }

    serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    enable_serial_interrupt();
}

void SerialSetTermChar(char ch) { term_char = ch; }


void enable_serial_interrupt(void)
{
    __disable_irq();

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    EXTI->IMR    |= EXTI_IMR_MR25;
    USART1->CR1  |= USART_CR1_RXNEIE;

    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);

    __enable_irq();
}

void USART1_EXTI25_IRQHandler(void)
{
    if (USART1->ISR & USART_ISR_RXNE) {
        char c = (char)USART1->RDR;
        handle_rx_char(c, &USART1_PORT);
    }
}




