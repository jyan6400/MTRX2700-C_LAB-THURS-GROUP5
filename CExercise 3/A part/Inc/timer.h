#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialise timer to call `callback` every `ms` milliseconds
void timer_init(uint16_t ms, void (*callback)(void));

#endif
