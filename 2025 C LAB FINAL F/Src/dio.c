//Relevant Header Files: Declare functions, variables, macros, and structures
#include "stm32f303xc.h"	//An STM32F303 official device header file
#include "dio.h"			//Digital i/o part header files


//Global static variable definition
static void (*button_callback)(void) = 0;	//Button interrupt callback function pointer
static uint8_t led_state = 0;				//Current LED status (each represents one LED on/off)


//Initialize button and LED GPIO
void dio_init(void (*callback)(void)) {
    //Save the button callback
    button_callback = callback;
    //Enable clocks for GPIOA (button) and GPIOE (LEDs)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;
    //Set PE8–PE15 as outputs
    for (int i = 8; i <= 15; i++) {
        GPIOE->MODER &= ~(3 << (2 * i));  // Clear
        GPIOE->MODER |=  (1 << (2 * i));  // Output
    }
    //Enable SYSCFG for EXTI
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    //Configure PA0 as EXTI0
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;  // 0 = PA0
    EXTI->RTSR |= EXTI_RTSR_TR0;
    EXTI->IMR  |= EXTI_IMR_MR0;
    //Configure the interrupt priority and enable the interrupt
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}


//Interrupt handler (triggered when button is pressed)
void EXTI0_IRQHandler(void) {
    if (button_callback) {
        button_callback();	//Callback function
    }
    EXTI->PR |= EXTI_PR_PR0;
}


//Set the status of the specified LED (index: 0~7)
void dio_setLED(int index, bool state) {
    if (index < 0 || index > 7) return;
    //Calculate the bitmask of PE8 to PE15
    uint16_t bit = 1 << (8 + index);

    if (state) {
    	//Output high level (light LED)
    	GPIOE->ODR |= bit;
        led_state |= (1 << index);
    } else {
    	//Output low level (extinguish LED)
    	GPIOE->ODR &= ~bit;
        led_state &= ~(1 << index);
    }
}


//Flip the status of the specified LED
void dio_toggleLED(int index) {
    if (index < 0 || index > 7) return;
    uint16_t bit = 1 << (8 + index);
    GPIOE->ODR ^= bit;
    led_state ^= (1 << index);
}


//Get the current 8 LED status (get led bitmask)
uint8_t dio_getLEDState(void) {
    return led_state;
}
