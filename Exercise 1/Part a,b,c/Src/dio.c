#include "stm32f303xc.h"
#include "dio.h"

static void (*button_callback)(void) = 0;
static uint8_t led_state = 0;

void dio_init(void (*callback)(void)) {
    // Save the button callback
    button_callback = callback;

    // Enable clocks for GPIOA (button) and GPIOE (LEDs)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;

    // Set PE8–PE15 as outputs
    for (int i = 8; i <= 15; i++) {
        GPIOE->MODER &= ~(3 << (2 * i));  // Clear
        GPIOE->MODER |=  (1 << (2 * i));  // Output
    }

    // Enable SYSCFG for EXTI
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure PA0 as EXTI0
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;  // 0 = PA0

    EXTI->RTSR |= EXTI_RTSR_TR0;
    EXTI->IMR  |= EXTI_IMR_MR0;

    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {
    if (button_callback) {
        button_callback();
    }
    EXTI->PR |= EXTI_PR_PR0;
}

void dio_setLED(int index, bool state) {
    if (index < 0 || index > 7) return;

    uint16_t bit = 1 << (8 + index);

    if (state) {
        GPIOE->ODR |= bit;
        led_state |= (1 << index);
    } else {
        GPIOE->ODR &= ~bit;
        led_state &= ~(1 << index);
    }
}

void dio_toggleLED(int index) {
    if (index < 0 || index > 7) return;

    uint16_t bit = 1 << (8 + index);
    GPIOE->ODR ^= bit;
    led_state ^= (1 << index);
}

uint8_t dio_getLEDState(void) {
    return led_state;
}
