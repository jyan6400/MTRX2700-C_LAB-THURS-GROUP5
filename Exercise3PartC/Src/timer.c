#include "stm32f303xc.h"
#include "timer.h"

static void (*oneshot_callback)(void) = 0;
static uint8_t oneshot_active = 0;

void timer_startOneShot(uint16_t ms, void (*callback)(void)) {
    // Enable TIM2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Reset and configure TIM2
    TIM2->CR1 = 0;
    TIM2->CNT = 0;

    // 8 MHz / 8000 = 1 kHz → 1 ms tick
    TIM2->PSC = 8000 - 1;
    TIM2->ARR = ms;

    // Store callback and enable one-shot flag
    oneshot_callback = callback;
    oneshot_active = 1;

    // Enable interrupt and start timer
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->EGR |= TIM_EGR_UG;       // Force update to load ARR
    TIM2->CR1 |= TIM_CR1_CEN;      // Start timer

    // Enable TIM2 interrupt in NVIC
    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;   // Clear interrupt flag

        if (oneshot_active) {
            oneshot_active = 0;
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop timer

            if (oneshot_callback) {
                oneshot_callback();
                oneshot_callback = 0;
            }
        }
    }
}
