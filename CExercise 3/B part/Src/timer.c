#include "stm32f303xc.h"
#include "timer.h"

static void (*timer_callback)(void) = 0;
static uint16_t timer_period_ms = 0;

void timer_init(uint16_t ms, void (*callback)(void)) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->CR1 = 0;
    TIM2->CNT = 0;
    TIM2->PSC = 8000 - 1;  // 1 ms tick
    TIM2->ARR = ms;
    timer_period_ms = ms;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CR1 |= TIM_CR1_CEN;

    timer_callback = callback;

    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void timer_setPeriod(uint16_t ms) {
    timer_period_ms = ms;
    TIM2->ARR = ms;
}

uint16_t timer_getPeriod(void) {
    return timer_period_ms;
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        if (timer_callback) {
            timer_callback();
        }
    }
}
