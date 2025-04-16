#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>


struct _SerialPort;
typedef struct _SerialPort SerialPort;

extern SerialPort USART1_PORT;


enum {
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200
};


void SerialInitialise(uint32_t baudRate,
                      SerialPort *serial_port,
                      void (*completion_function)(char *, uint32_t));

void SerialSetTermChar(char ch);
void SerialPrintPrompt(SerialPort *serial_port);
void SerialPrintLastLength(SerialPort *serial_port);

void SerialOutputChar(uint8_t data, SerialPort *serial_port);
void SerialOutputString(uint8_t *pt, SerialPort *serial_port);


void enable_serial_interrupt(void);

#endif
