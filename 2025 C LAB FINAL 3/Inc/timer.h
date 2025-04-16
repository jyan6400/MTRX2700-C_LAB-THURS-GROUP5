#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* ─── API ──────────────────────────────────────────────── */
/* 周期性定时器：每 period_ms 毫秒触发一次 callback */
void timer_init(uint32_t period_ms, void (*callback)(void));

/* 一次性定时器：等待 delay_ms 毫秒，仅触发一次 callback */
void timer_oneshot(uint32_t delay_ms, void (*callback)(void));

#endif
