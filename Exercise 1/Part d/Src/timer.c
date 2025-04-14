#include "stm32f303xc.h"
#include "partd_io.h"


static volatile uint8_t allow_led_update = 0;

void timer_init(uint16_t ms) {
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Reset timer
    TIM2->CR1 = 0;
    TIM2->CNT = 0;

    // If system clock is 8 MHz:
    // 8,000,000 / 8000 = 1000 Hz → 1 tick per ms
    TIM2->PSC = 8000 - 1;  // 8 MHz → 1 kHz (1 tick per ms)
    TIM2->ARR = ms;        // match desired interval in ms

    TIM2->SR &= ~TIM_SR_UIF;     // Clear any existing update interrupt flag
    TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;    // Start timer

    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;   // Clear interrupt flag
        allow_led_update = 1;      // Set flag for main code
    }
}

uint8_t timer_check_flag(void) {
    return allow_led_update;
}

void timer_clear_flag(void) {
    allow_led_update = 0;
}
