//Relevant Header Files
#include <stdint.h>		//Standard integer type definition header file
#include <stdio.h>		//Standard I/O library header file
#include <stdlib.h>		//Standard library function header file
#include <string.h>		//String function header file
#include "serial.h"		//Serial part header file

//The callback function after the serial port is received
void For_receive_done(uint8_t *data, uint32_t len) {
    SerialOutputString((uint8_t *)"\r\n", &USART1_PORT);
    SerialOutputString(data, &USART1_PORT);
    SerialOutputString((uint8_t *)"\r\n...Received successfully, printing input...\r\n", &USART1_PORT);
}

//Main function
int main(void) {
	SerialInitialise(BAUD_115200, &USART1_PORT, For_receive_done);
	SerialSetTermChar('#');
    SerialPrintPrompt(&USART1_PORT);
    Enable_Serial_Interrupt();
//The while loop is currently empty and can be added with other processing in the future
    while (1) {
    }
}
