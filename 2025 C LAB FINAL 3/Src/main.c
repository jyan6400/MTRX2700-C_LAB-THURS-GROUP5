
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "stm32f303xc.h"
#include "serial.h"
#include "dio.h"
#include "timer.h"

volatile uint16_t last_received_length = 0;


static void on_timer_callback(void);
static void flash_led_once(void);


static void process_led_command(const char *operand)
{
    size_t len = strlen(operand);
    if (len == 0 || len > 8) return;

    for (int i = 0; i < 8; ++i) {
        bool bit = 0;
        int src = (int)len - 1 - i;
        if (src >= 0 && (operand[src] == '1' || operand[src] == '0'))
            bit = (operand[src] == '1');
        dio_setLED(i, bit);
    }
}


static void process_serial_command(const char *operand)
{
    SerialOutputString((uint8_t *)operand, &USART1_PORT);
    SerialOutputString((uint8_t *)"\r\n",   &USART1_PORT);
}


static void process_timer_command(const char *operand)
{
    int period_ms = atoi(operand);
    if (period_ms > 0)
        timer_init((uint32_t)period_ms, on_timer_callback);
}


static void process_oneshot_command(const char *operand)
{
    int delay_ms = atoi(operand);
    if (delay_ms > 0)
        timer_oneshot((uint32_t)delay_ms, flash_led_once);
}


static void on_timer_callback(void)
{
    dio_toggleLED(0);
}


static void flash_led_once(void)
{
    dio_setLED(0, true);
    for (volatile uint32_t i = 0; i < 400000; ++i) { __NOP(); }
    dio_setLED(0, false);
}


void on_receive_done(char *data, uint32_t len)
{
    last_received_length = (uint16_t)len;
    while (*data == ' ') ++data;

    if (strncasecmp(data, "led ", 4) == 0)
        process_led_command(data + 4);
    else if (strncasecmp(data, "serial ", 7) == 0)
        process_serial_command(data + 7);
    else if (strncasecmp(data, "timer ", 6) == 0)
        process_timer_command(data + 6);
    else if (strncasecmp(data, "oneshot ", 8) == 0)
        process_oneshot_command(data + 8);
}

int main(void)
{
    dio_init(NULL);
    SerialInitialise(BAUD_115200, &USART1_PORT, on_receive_done);
    SerialSetTermChar('#');
    SerialPrintPrompt(&USART1_PORT);

    while (1) {

    }
}
