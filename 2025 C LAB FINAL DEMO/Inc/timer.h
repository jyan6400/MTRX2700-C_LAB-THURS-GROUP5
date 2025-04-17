#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

//Initialise timer to call `callback` every `ms` milliseconds
void timer_init(uint32_t period_ms, void (*callback)(void));

//One-shot timer that triggers `callback` after `ms` milliseconds
void timer_oneshot(uint32_t delay_ms, void (*callback)(void));

//Set a new period (in ms)
void timer_setPeriod(uint32_t period_ms);

//Get the current period
uint32_t timer_getPeriod(void);

//Stop the TIM2 timer and close the interrupt
void timer_stop(void);

#endif
