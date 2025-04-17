#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include <stdbool.h>

// Initialise button and LEDs; pass callback for button ISR
void dio_init(void (*callback)(void));

// Set LED state (PE8–PE15: index 0–7)
void dio_setLED(int index, bool state);

// Toggle LED at index (PE8 = 0)
void dio_toggleLED(int index);

// Get current LED state as bitmask
uint8_t dio_getLEDState(void);

#endif
