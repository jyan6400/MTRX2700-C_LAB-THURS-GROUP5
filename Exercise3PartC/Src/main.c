#include "stm32f303xc.h"

int main(void) {
    // Setup PE9
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    GPIOE->MODER &= ~(3 << (2 * 9)); // Clear mode bits
    GPIOE->MODER |=  (1 << (2 * 9)); // Set as output
    GPIOE->ODR &= ~(1 << 9);         // PE9 OFF initially

    // Setup TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 8000 - 1;   // 1 ms per tick (assuming 8 MHz)
    TIM2->ARR = 0xFFFF;
    TIM2->CNT = 0;

    TIM2->EGR |= TIM_EGR_UG;   // Force update
    TIM2->CR1 |= TIM_CR1_CEN;  // Start timer

    while (1) {
        if (TIM2->CNT > 7000) {
            GPIOE->ODR |= (1 << 9);  // Turn on PE9
        }
    }
}
