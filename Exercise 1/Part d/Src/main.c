#include "stm32f303xc.h"
#include <stdint.h>
#include "partd_io.h"

void chase_led() {
    uint8_t current = led_get();
    current = (current == 0 || current >= 0x80) ? 0x01 : current << 1;
    led_set(current);
}

int main(void) {
    led_init();
    button_init(chase_led);
    timer_init(1000);  // LED can update only every 200ms

    while (1) {
        // wait for interrupts
    }
}
