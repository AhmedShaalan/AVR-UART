//
// Created by Ahmed Shaalan on May 25, 2020.
//

#include <avr/io.h>

#ifndef UART_BAUD_RATE_PRESCALE
#define UART_BAUD_RATE_PRESCALE 51 // to get 9,600 Baud
#endif

void UART_initRxTx(void);
void UART_initTx(void);
void UART_initRx(void);
void UART_sendChar(uint8_t data);
void UART_sendLine(uint8_t* ptrString);
uint8_t UART_readChar(void);