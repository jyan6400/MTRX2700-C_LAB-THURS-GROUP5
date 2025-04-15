#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialise timer to call `callback` every `ms` milliseconds
void timer_init(uint16_t ms, void (*callback)(void));

// Set a new period (in ms)
void timer_setPeriod(uint16_t ms);

// (optional) Get the current period
uint16_t timer_getPeriod(void);

#endif
