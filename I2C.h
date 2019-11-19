/*
 * I2C.h
 *
 *  Created on: Nov 13, 2019
 *      Author: User01
 */

#ifndef I2C_H_
#define I2C_H_

#define SDA BIT6
#define SCL BIT7
#define START_BIT   EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT      //enable start bit
#define DIS_START_BIT  EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TXSTT
#define STOP_BIT    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP      //enable stop bit
#define DIS_STOP_BIT    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TXSTP
#define ACK         EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXACK      //send ACK
#define NACK        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXNACK     //send NACK
#define TRANSMITTER EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR         //in transmit mode
#define RECEIVER    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR        //in receive mode
#define CLEAR_TX_FLAG    EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0    //clear TX flag
#define CLEAR_RX_FLAG    EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG0    //clear RX flag

void EUSCI_B0_I2C_setup();
void I2C_slave_addr(uint8_t address_);
void EEPROM_mem_addr(uint16_t mem_addr);
void I2C_Write_Char(uint8_t buffer);
char I2C_Read_Char();

#endif /* I2C_H_ */
