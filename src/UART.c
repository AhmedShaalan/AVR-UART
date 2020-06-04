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
#include "uart.h"
#include <avr/interrupt.h>

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
    // TODO: how we are going to tell when to
    //   call the function that is intented to call when the interrupt is complete?

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

uint8_t UART_readChar(void) {

    // wait till receive is completed
    while (!(UCSRA & (1 << RXC)))
        ;

    // return received data
    return UDR;
}

ISR(USART_RXC_vect) {
}

ISR(USART_TXC_vect) {
}