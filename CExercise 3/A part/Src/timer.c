#include "stm32f303xc.h"
#include "timer.h"

static void (*timer_callback)(void) = 0;

void timer_init(uint16_t ms, void (*callback)(void)) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->CR1 = 0;
    TIM2->CNT = 0;
    TIM2->PSC = 8000 - 1;  // 8 MHz / 8000 = 1 kHz (1 tick = 1 ms)
    TIM2->ARR = ms;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CR1 |= TIM_CR1_CEN;

    timer_callback = callback;

    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;

        if (timer_callback) {
            timer_callback();
        }
    }
}
