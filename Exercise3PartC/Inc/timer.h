#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// One-shot timer that triggers `callback` after `ms` milliseconds
void timer_startOneShot(uint16_t ms, void (*callback)(void));

#endif
