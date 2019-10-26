/*
 * SPI.h
 *
 *  Created on: Oct 16, 2019
 *      Author: User01
 */

#ifndef SPI_H_
#define SPI_H_

#define DAC_CS   BIT3     //P10.3
#define GAIN     BIT5
#define SHDN     BIT4
#define SPI_CLK  BIT5
#define SPI_MOSI BIT6
#define SPI_MISO BIT7


void EUSCI_B0_setup();
void clear_EUSCI_B0_reg();
void DAC_output(uint16_t outVolt);

uint16_t voltage_bit;


#endif /* SPI_H_ */
