#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

//Callback function after the serial port receives information
void on_receive_done(uint8_t *data, uint32_t len);

#endif
