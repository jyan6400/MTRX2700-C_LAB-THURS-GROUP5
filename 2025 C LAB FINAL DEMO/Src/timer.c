//Relevant Header Files: Declare functions, variables, macros, and structures
#include "stm32f303xc.h"	//An STM32F303 official device header file
#include "timer.h"			//timer part header files


//Defines a static function pointer
static void (*timer_callback)(void) = 0;
//Define global static variables
static uint32_t timer_period_ms = 0;


//Configure and start TIM2
static void tim2_config_start(uint32_t ms, int oneshot)
{
	//Enabled the TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 = 0;
    TIM2->CNT = 0;
    //Set the prescale value so that the timing is in ms
    TIM2->PSC = 8000 - 1;
    TIM2->ARR = (uint16_t)ms;

    TIM2->EGR = TIM_EGR_UG;
    // Clear the flag
    TIM2->SR  = 0;
    // Allow update interrupt
    TIM2->DIER = TIM_DIER_UIE;
    if (oneshot)
        TIM2->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;
    else
        TIM2->CR1 = TIM_CR1_CEN;
    //Interrupt the priority configuration
    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}


//Periodic timer that automatically uses callback function when interrupted
void timer_init(uint32_t period_ms, void (*callback)(void))
{
    timer_callback  = callback;
    timer_period_ms = period_ms;
    tim2_config_start(period_ms, 0);
}


//TIM2 (one-shot) timer, Only execute the callback once the delay is over
void timer_oneshot(uint32_t delay_ms, void (*callback)(void))
{
    timer_callback = callback;
    tim2_config_start(delay_ms, 1);
}


//Update the cycle time of the current timer
void timer_setPeriod(uint32_t period_ms)
{
    timer_period_ms = period_ms;
    tim2_config_start(period_ms, 0);
}


//Period of the current timer
uint32_t timer_getPeriod(void)
{
    return timer_period_ms;
}


//Timer stop function
void timer_stop(void)
{
    //Stop counting
	TIM2->CR1  = 0;
    TIM2->DIER = 0;
    NVIC_DisableIRQ(TIM2_IRQn);
    //Clear all flags
    TIM2->SR   = 0;
    timer_callback = 0;
}


//TIM2 global interrupt handler
void TIM2_IRQHandler(void)
{
	//Check UIF bit for TIM2 Whether set
    if (TIM2->SR & TIM_SR_UIF) {
    	//Clearing UIF
    	TIM2->SR &= ~TIM_SR_UIF;
        //Callback function
    	if (timer_callback) {
            timer_callback();
        }
    }
}
