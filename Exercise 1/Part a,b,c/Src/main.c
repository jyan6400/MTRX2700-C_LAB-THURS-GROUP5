#include "dio.h"

void chase_led() {
    uint8_t current = dio_getLEDState();
    current = (current == 0 || current >= 0x80) ? 0x01 : current << 1;

    for (int i = 0; i < 8; i++) {
        dio_setLED(i, (current >> i) & 1);
    }
}

int main(void) {
    dio_init(chase_led);

    while (1) {
        // waiting for interrupt
    }
}
