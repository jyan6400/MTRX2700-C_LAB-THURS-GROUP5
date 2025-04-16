//Relevant Header Files
#include <stdint.h>			//Standard integer type definition header file
#include <string.h>			//String function header file
#include <stdlib.h>			//Standard library function header file
#include "stm32f303xc.h"	//Device header file for STM32F303 series MCUS
#include "serial.h"			//Serial part header file
#include "dio.h"			//digital io part header file
#include "timer.h"			//timer part header file
#include "command.h"		//command part header file

//Main function
int main(void)
{
    SerialInitialise(BAUD_115200, &USART1_PORT, on_receive_done);
    SerialSetTermChar('#');
    SerialPrintPrompt(&USART1_PORT);
    Enable_Serial_Interrupt();
    dio_init(NULL);
}
