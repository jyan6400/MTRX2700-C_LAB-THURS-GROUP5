//Relevant Header Files: Declare functions, variables, macros, and structures
#include "serial.h"			//Serial part header files
#include "stm32f303xc.h"	//An STM32F303 official device header file
#include <string.h>         //String function header file
#include <stdio.h>			//Standard input/output header file


//Conditional compilation check block, detects whether a floating point unit (FPU)
//is enabled without being initialized
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


//Variable definition
#define BUFFER_SIZE 1000				// Define a buffer size of 1000 bytes
static uint8_t  rx_buffer[BUFFER_SIZE];
static uint8_t rx_index = 0;			// Receive buffer index
static char  term_char = '\r';

//We store the pointers to the GPIO and USART that are used for a specific
//serial port. To add another serial port you need to select the appropriate values.
struct _SerialPort {
    USART_TypeDef *UART;
    GPIO_TypeDef *GPIO;
    volatile uint32_t MaskAPB2ENR;
    volatile uint32_t MaskAPB1ENR;
    volatile uint32_t MaskAHBENR;
    volatile uint32_t SerialPinModeValue;
    volatile uint32_t SerialPinSpeedValue;
    volatile uint32_t SerialPinAlternatePinValueLow;
    volatile uint32_t SerialPinAlternatePinValueHigh;
    void (*completion_function)(uint8_t *, uint32_t);;
};


//Instantiate the serial port parameters note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, 		// bit to enable for APB2 bus
		0x00,						// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, 		// bit to enable for AHB bus
		0xA00,
		0xF00,
		0x770000,  					// for USART1 PC10 and 11, this is in the AFR low register
		0x00, 						// no change to the high alternate function register
		0x00 						// default function pointer is NULL
		};


// InitialiseSerial - Initialize the serial port, input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint8_t *, uint32_t)) {
	serial_port->completion_function = completion_function;
	// enable clock power, system configuration clock and GPIOC common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;
	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;
	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;
	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;
	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;
	// Get a pointer to the 16 bits of the BRR register that we want to change
    uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR;
    // Baud rate at 8M Hz
    switch (baudRate) {
    case BAUD_9600:
    	// BRR(USARTDIV): 833 = 0x341
    	*baud_rate_config = 8000000 / 9600;
    	break;
    case BAUD_19200:
    	// BRR(USARTDIV): 416 = 0x1A0
    	*baud_rate_config = 8000000 / 19200;
        break;
    case BAUD_38400:
    	// BRR(USARTDIV): 208 = 0xD0
    	*baud_rate_config = 8000000 / 38400;
        break;
    case BAUD_57600:
    	// BRR(USARTDIV): 139 = 0x8B
    	*baud_rate_config = 8000000 / 57600;
        break;
    case BAUD_115200:
    	// BRR(USARTDIV): 69.44 ≈ 0x46
    	*baud_rate_config = 8000000 / 115200;
        break;
	}
    serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


//Set terminating character
void SerialSetTermChar(char terminator_ch) {
    term_char = terminator_ch;
}



//Send a character data to the serial port
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
    while ((serial_port->UART->ISR & USART_ISR_TXE) == 0) {}
    serial_port->UART->TDR = data;
}


//Sends a string characters
void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {
    while (*pt) {
        SerialOutputChar(*pt, serial_port);
        pt++;
    }
}


//Print user input prompt to the serial port
void SerialPrintPrompt(SerialPort *serial_port) {
    static int prompt_shown = 0;
    if (!prompt_shown) {
        SerialOutputString((uint8_t *)"User input interface (end with '#'):\r\n", serial_port);
        prompt_shown = 1;
    }
}


//Received incoming user input character
void SerialInputReceive(char c, SerialPort *serial_port) {
	//Check whether the buffer overflow
    if (rx_index < BUFFER_SIZE - 1) {	//Leave space in the buffer
    	rx_buffer[rx_index++] = c;		//Store the character and index + 1
    } else {
    	//Overflow information
    	SerialOutputString((uint8_t *)"[ERROR] Buffer Overflow!\r\n", serial_port);
        rx_index = 0;
        memset(rx_buffer, 0, BUFFER_SIZE);
        return;
    }
    //Check if it is a terminator.
    if (c == term_char) {
        rx_buffer[rx_index] = '\0';
        //Completion callback
        if (serial_port->completion_function) {
            serial_port->completion_function(rx_buffer, rx_index - 1);
        }
        rx_index = 0;
    }
}

//Enable the function for the serial port to interrupt
void Enable_Serial_Interrupt(void) {
    __disable_irq();
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    EXTI->IMR |= EXTI_IMR_MR25;
    USART1->CR1 |= USART_CR1_RXNEIE;
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);
    __enable_irq();
}

//USART1 interrupt handler function
void USART1_EXTI25_IRQHandler(void) {
	//Check whether RXNE interrupt flag is displayed
	if (USART1->ISR & USART_ISR_RXNE) {
		//Reads a byte from the receive data register
		char c = (char)USART1->RDR;
		//Jump to  Input receive function and pass in the read characters for processing
		SerialInputReceive(c, &USART1_PORT);
    }
}
