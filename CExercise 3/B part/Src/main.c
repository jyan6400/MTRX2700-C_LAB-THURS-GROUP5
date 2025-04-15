#include "stm32f303xc.h"
#include "timer.h"

void toggle_led() {
    GPIOE->ODR ^= (1 << 8);  // Toggle PE8

    // Every time it toggles, change the period
    static int toggle = 0;
    toggle = !toggle;

    if (toggle) {
        timer_setPeriod(1000);
    } else {
        timer_setPeriod(5000);
    }
}

int main(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    GPIOE->MODER &= ~(3 << (2 * 8));
    GPIOE->MODER |=  (1 << (2 * 8));

    timer_init(1000, toggle_led);

    while (1) {}
}
