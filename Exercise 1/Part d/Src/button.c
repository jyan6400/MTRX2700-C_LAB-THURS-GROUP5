#include "stm32f303xc.h"
#include "partd_io.h"


static void (*button_callback)(void) = 0;

void EXTI0_IRQHandler(void) {
    if (button_callback) {
        button_callback();
    }

    // Clear pending interrupt
    EXTI->PR |= EXTI_PR_PR0;
}

void button_init(void (*callback)()) {
    button_callback = callback;

    // Enable GPIOA clock for the button (PA0)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // PA0 is already input by default, so no need to set MODER

    // Enable SYSCFG clock for EXTI mapping
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Map PA0 to EXTI0
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;  // 0b0000 = PA0

    // Enable rising edge trigger
    EXTI->RTSR |= EXTI_RTSR_TR0;

    // Unmask EXTI0
    EXTI->IMR |= EXTI_IMR_MR0;

    // Enable EXTI0 IRQ in NVIC
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}
