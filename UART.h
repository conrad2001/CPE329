/*
 * UART.h
 *
 *  Created on: Oct 24, 2019
 *      Author: User01
 */

#ifndef UART_H_
#define UART_H_

/*
UART- Universal asynchronous receiver transmitter
Asynchronous - no CLK signal
        - less wire / pin
Preconfigured Timing
    Baud rate - speed of sending bits / second
    Typical range 300 bps - 115.2 kbps
Oversampling
Modulation

fBRCLK - EUSCI-A, clock = 3MHz
Baud rate 115.2kbps
N= 3MHz / 115.2kbps = 26.04166
N > 16 => oversample
OS16 = 1
UCBRx = INT(N/16) = INT(1.6276) = 1
UCBRF = INF([ 1.6276 - 1 ] x 16) = 10
UCBRSx = set from table using fractional part of N
0.04166=> 0x0
 */

#define A0_RX BIT2
#define A0_TX BIT3
#define UCBRS 1  //INT(N/16) = INT(1.6276) = 1
#define UCBRFx 10 //INF([ 1.6276 - 1 ] x 16) = 10
/*set from table using fractional part of N 0.04166 => 0*/
#define UCBRSx 0

void EUSCI_A0_UART_setup();
uint16_t return_DAC_value();
/*void Read_UART(char* _buffer, int length);
void Write_UART(char* _buffer);*/

#endif /* UART_H_ */
