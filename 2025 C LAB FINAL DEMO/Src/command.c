//Relevant Header Files
#include <string.h>         // String function header file
#include <stdlib.h>         // Standard library function header file
#include "stm32f303xc.h"    // Device header file for STM32F303 series MCUs
#include "serial.h"         // Serial part header file
#include "dio.h"            // Digital IO part header file
#include "timer.h"          // Timer part header file
#include "command.h"        // Command part header file


//Internal LED utility function: Turn off all LEDs
static inline void leds_off(void)
{
	//Turn off all the LEDs
	for (int i = 0; i < 8; ++i) dio_setLED(i, false);
}


//One-shot timer callback function, the LED will flash once
static void flash_all_once(void)
{
	//Turn on all the LEDs
	for (int i = 0; i < 8; ++i) dio_setLED(i, true);
	//Simple implementation of blinking through delay anbout 50 ms
	for (volatile uint32_t d = 0; d < 400000; ++d) __NOP();
	//Turn off all the LEDs
	leds_off();
}


//timer Periodic Timer Callback Function,Perform flashing
static void blink_toggle_all(void)
{
	//Toggle the states one by one to achieve the blinking effect.
	for (int i = 0; i < 8; ++i) dio_toggleLED(i);
}


//Serial port data judgment callback function
void For_receive_done(uint8_t *data, uint32_t len)
{
    // The double buffering is set in the serial interface
	//When the received data is processed by using ps_buffer
    data[len] = '\0';  //Append \0 at the end of the string

    //Start the search from the last character of the string and move backward
    for (int i = len - 1; i >= 0; --i) {
        //Replace all illegal characters at the end of the parameters with '\0' for processing
    	if (data[i] == '#' || data[i] == '\r' || data[i] == '\n') {
            data[i] = '\0';		//Append \0 at the end of the string.
        } else {
            break;
        }
    }

    //Use the strtok function to split the string. When encountering a space character, convert it to '\0'.
    //Then extract "cmd" as the command. "arg" refers to a parameter.
    char *cmd = strtok((char *)data, " ");    //Command
    char *arg = strtok(NULL, "");             //Parameter
    //If the command is empty
    if (!cmd) {
        //Output error message prompt information
    	SerialOutputString((uint8_t *)"\r\n[ERROR] Empty command\r\n", &USART1_PORT);
        return;
    }

    //LED operation command: When the user inputs, the LED will turn on the
	//corresponding LED according to the parameters.
    if (strcmp(cmd, "led") == 0) {
        //Stop the current timer (to prevent conflict with LED flashing)
    	timer_stop();
        //Check whether the parameters are 8 bits in total and each bit is either 0 or 1.
    	if (!arg || strlen(arg) != 8) {
            //Output error message prompt information
    		SerialOutputString((uint8_t *)"\r\n[ERROR] led expects 8-bit binary\r\n", &USART1_PORT);
            return;
        }
        //Check string
    	for (int i = 0; i < 8; ++i) {
    		//Check whether the parameters each bit is either 0 or 1.
    		if (arg[i] != '0' && arg[i] != '1') {
    			//Output error message prompt information
    			SerialOutputString((uint8_t *)"\r\n[ERROR] led pattern must be 0/1\r\n", &USART1_PORT);
                return;
            }
    		//Turn on the LED according to the parameters.
    		dio_setLED(7 - i, arg[i] == '1');
        }
        SerialOutputString((uint8_t *)"\r\n[Successful] LED pattern set\r\n", &USART1_PORT);
    }



    //Serial part command: When the user enters serial,
    //the system checks and displays the character string entered by the user.
    else if (strcmp(cmd, "serial") == 0) {
    	//Performs line feed and outputs the corresponding parameters
    	SerialOutputString((uint8_t *)"\r\n", &USART1_PORT);
        if (arg) {
            SerialOutputString((uint8_t *)arg, &USART1_PORT);
        }
        SerialOutputString((uint8_t *)"\r\n", &USART1_PORT);
    }


    //One shot part command:Execute one-time delay.
    else if (strcmp(cmd, "oneshot") == 0) {
        //If the parameter is null or incorrect, an error message is displayed
    	if (!arg) {
            SerialOutputString((uint8_t *)"\r\n[ERROR] oneshot expects delay\r\n", &USART1_PORT);
            return;
        }
        //Converts a string argument to an integer, in milliseconds
    	//atoi function converts a parameter string to an integer
    	uint32_t delay = (uint32_t)atoi(arg);
    	//Stop existing timers and turn off all leds
        timer_stop();
        leds_off();
        //Execute the one shot function to implement a one-time delay
        timer_oneshot(delay, flash_all_once);
        SerialOutputString((uint8_t *)"\r\n[Successful] oneshot set\r\n", &USART1_PORT);
    }


    //timer part command:Execute delay and flash leds
    else if (strcmp(cmd, "timer") == 0) {
        //Check that the parameters provided are reasonable and non-null
    	if (!arg) {
            //Output a error message
    		SerialOutputString((uint8_t *)"\r\n[ERROR] timer expects period\r\n", &USART1_PORT);
            return;
        }
    	//Converts a string argument to an integer, in milliseconds
    	//atoi function converts a parameter string to an integer
    	uint32_t period = (uint32_t)atoi(arg);
    	//Stop the old timer and turn off all leds
        timer_stop();
        leds_off();
        //Set a periodic timer, achieve periodic reversal led
        timer_init(period, blink_toggle_all);
        SerialOutputString((uint8_t *)"\r\n[Successful] timer started\r\n", &USART1_PORT);
    }

    //An unrecognized command: prompts an error message
    else {
        SerialOutputString((uint8_t *)"\r\n[ERROR] Unknown command\r\n", &USART1_PORT);
    }
}
