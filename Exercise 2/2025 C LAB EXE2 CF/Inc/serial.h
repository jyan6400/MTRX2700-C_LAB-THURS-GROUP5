#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

// Defining the serial port struct, the definition is hidden in the
// c file as no one really needs to know this.
struct _SerialPort;
typedef struct _SerialPort SerialPort;

// make any number of instances of the serial port (they are
// extern because they are fixed, unique values)
extern SerialPort USART1_PORT;

// The user might want to select the baud rate
enum {
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200
};

void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint8_t *, uint32_t));
void SerialOutputChar(uint8_t data, SerialPort *serial_port);
void SerialOutputString(uint8_t *str, SerialPort *serial_port);


void SerialInputReceive(char c, SerialPort *serial_port);
void SerialSetTermChar(char terminator_ch);
void SerialPrintPrompt(SerialPort *serial_port);
void Enable_Serial_Interrupt(void);
#endif
