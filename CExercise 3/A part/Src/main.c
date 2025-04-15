#include "stm32f303xc.h"
#include "timer.h"

void toggle_led() {
    GPIOE->ODR ^= (1 << 8);  // Toggle PE8
}

int main(void) {
    // Enable GPIOE and set PE8 as output
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    GPIOE->MODER &= ~(3 << (2 * 8));
    GPIOE->MODER |=  (1 << (2 * 8));

    // Initialise timer to call `toggle_led` every 1000 ms
    timer_init(1000, toggle_led);

    while (1) {
        // Nothing here - all handled by timer interrupt
    }
}
