//Relevant Header Files
#include <stdint.h>		//Standard integer type definition header file
#include <stdio.h>		//Standard I/O library header file
#include <stdlib.h>		//Standard library function header file
#include <string.h>		//String function header file
#include "serial.h"		//Serial part header file


volatile uint16_t last_received_length = 0;

//The callback function after the serial port is received
void For_receive_done(uint32_t len) {
    last_received_length = (uint16_t)len;
}

//Main function
int main(void) {
    SerialInitialise(BAUD_115200, &USART1_PORT, For_receive_done);
    SerialSetTermChar('#');
    SerialPrintPrompt(&USART1_PORT);
    //Use polling method to received the input
    while (1) {
        SerialInputReceive(&USART1_PORT);
        SerialPrintLastLength(&USART1_PORT);
    }
}
