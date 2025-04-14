#include "stm32f303xc.h"
#include "partd_io.h"

static uint8_t led_state = 0;

void led_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

    for (int i = 8; i <= 15; i++) {
        GPIOE->MODER &= ~(3 << (2 * i));
        GPIOE->MODER |=  (1 << (2 * i));
    }
}

void led_set(uint8_t value) {
    if (timer_check_flag()) {
        led_state = value;
        GPIOE->ODR &= ~(0xFF << 8);
        GPIOE->ODR |= (led_state << 8);
        timer_clear_flag();
    }
}

uint8_t led_get(void) {
    return led_state;
}
