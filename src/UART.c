/****************************************************************************
    UART.c  - AVR UART protocol library

    Copyright (C) 2020 Ahmed Shaalan    (ahmed_shaalan@icloud.com)

  ****************************************************************************
         File:    UART.c
         Date:    May 25, 2020
       Target:    ATmega32A
     Compiler:    avr-gcc
       Author:    Ahmed Shaalan

      Specs:
        - 1 Stop bit
        - Normal Speed (not 2x)
        - No parity check

  **************************************************************************/
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <LCD.h>
#include "uart.h"

#define UART_BUFFER_SIZE_MASK (UART_BUFFER_SIZE - 1)

#if (UART_BUFFER_SIZE & UART_BUFFER_SIZE_MASK)
#error UART_BUFFER_SIZE must be power of 2
#endif

static volatile uint8_t UART_rxBuf[UART_BUFFER_SIZE];
static volatile uint8_t UART_rxHead = 0;
static volatile uint8_t UART_rxTail = 0;

volatile int8_t UART_lastError = UART_ERROR_NO_ERROR;

void UART_initRxTx(void) {
    // TODO: enable global interrput in SREG; Bit 7 – I: Global Interrupt Enable.
    // TODO: enable Rx & Tx complete interrupt in UCSRA,
    // TODO:    Bit 6 – TXC: USART Transmit Complete &  Bit 7 – RXC: USART Receive Complete

    // put the upper part of the baud number prescale in the UBBRH (high 8 bits)
    UBRRH = (UART_BAUD_RATE_PRESCALE >> 8);

    // put the remaining part of the baud number prescale in the UBRRL
    UBRRL = UART_BAUD_RATE_PRESCALE;

    // enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);

    // set data bit length to 8-bit
    UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

void UART_initTx(void) {
    // TODO: enable global interrput in SREG; Bit 7 – I: Global Interrupt Enable.
    // TODO: enable Tx complete interrupt in UCSRA. Bit 6 – TXC: USART Transmit Complete

    // put the upper part of the baud number here (bits 8 to 11)
    UBRRH = (UART_BAUD_RATE_PRESCALE >> 8);

    // put the remaining part of the baud number here
    UBRRL = UART_BAUD_RATE_PRESCALE;

    // enable transmitter
    UCSRB = (1 << TXEN);

    // set data bit length to 8-bit
    UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

void UART_initRx(void) {

    // put the upper part of the baud number here (bits 8 to 11)
    UBRRH = (UART_BAUD_RATE_PRESCALE >> 8);

    // put the remaining part of the baud number here
    UBRRL = UART_BAUD_RATE_PRESCALE;

    // enable receiver & Rx complete interrupt
    UCSRB = (1 << RXEN) | (1 << RXCIE);

    // set data bit length to 8-bit
    UCSRC = (1 << URSEL) | (3 << UCSZ0);

    // enable global interrupt
    sei();
}

void UART_sendChar(uint8_t data) {

    // wait for previous data to be transmitted
    while (!(UCSRA & (1 << UDRE)))
        ;

    // send new data
    UDR = data;
}

void UART_sendLine(uint8_t* ptrString) {

    // print char by char from string array
    while (*ptrString) {
        UART_sendChar(*ptrString++);
    }

    // start new line
    UART_sendChar('\n');
}

ISR(USART_RXC_vect) {
    uint8_t tempHead;
    uint8_t data = UDR;

    // head position
    tempHead = (UART_rxHead + 1) & UART_BUFFER_SIZE_MASK;

    // check if we reached the tail. If true it means buffer is full
    if (tempHead == UART_rxTail) {
        UART_lastError = UART_ERROR_BUFFER_FULL;
    } else {
        // if not full, store Rx data.
        UART_rxBuf[tempHead] = data;

        // advance the head
        UART_rxHead = tempHead;
    }
}

ISR(USART_TXC_vect) {
}

uint8_t UART_getChar(void) {

    // pause interrupts till we check if the buffer is full
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        // check if buffer is empty
        if (UART_rxTail == UART_rxHead) {
            return UART_BUFFER_EMPTY;
        }
    }

    // advance the tail
    UART_rxTail = (UART_rxTail + 1) & UART_BUFFER_SIZE_MASK;

    return UART_rxBuf[UART_rxTail];
}