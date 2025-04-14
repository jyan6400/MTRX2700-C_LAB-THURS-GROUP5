#ifndef PARTD_IO_H
#define PARTD_IO_H

#include <stdint.h>

//
// ===== BUTTON =====
// Handles button interrupt (PA0 -> EXTI0)
//

void button_init(void (*callback)(void));


//
// ===== LED =====
// PE8–PE15 controlled using bitmask
//

void led_init(void);
void led_set(uint8_t value);
uint8_t led_get(void);


//
// ===== TIMER =====
// TIM2-based timer for periodic flag setting
//

void timer_init(uint16_t ms);
uint8_t timer_check_flag(void);
void timer_clear_flag(void);

#endif
