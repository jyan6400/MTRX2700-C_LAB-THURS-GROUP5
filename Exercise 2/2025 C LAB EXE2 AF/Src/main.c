//Relevant Header Files
#include <stdint.h>		//Standard integer type definition header file
#include <stdio.h>		//Standard I/O library header file
#include <stdlib.h>		//Standard library function header file
#include <string.h>		//String function header file
#include "serial.h"		//Serial part header file

//The callback function after the serial port is received
void callback(uint32_t len) {
    (void)len;
}

//Main function
int main(void) {
    SerialInitialise(BAUD_115200, &USART1_PORT, callback);
    SerialSetTermChar('#');
    SerialPrintPrompt(&USART1_PORT);
    //Use polling method to received the input
    while (1) {
        SerialInputReceive(&USART1_PORT);
    }
}
