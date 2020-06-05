//
// Created by Ahmed Shaalan on May 25, 2020.
//

#include <avr/io.h>

#ifndef UART_BAUD_RATE_PRESCALE
#define UART_BAUD_RATE_PRESCALE 51 // to get 9,600 Baud
#endif

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 32
#endif

#define UART_ERROR_NO_ERROR    -1
#define UART_ERROR_BUFFER_FULL 1

#define UART_BUFFER_EMPTY 0

void UART_initRxTx(void);
void UART_initTx(void);
void UART_initRx(void);
void UART_sendChar(uint8_t data);
void UART_sendLine(uint8_t* ptrString);
uint8_t UART_getChar(void);