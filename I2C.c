/*
 * I2C.c
 *
 *  Created on: Nov 13, 2019
 *      Author: User01
 */
#include "msp.h"
#include "I2C.h"
#include "delay_us.h"

uint8_t address;
uint16_t mem_addr_;

void EUSCI_B0_I2C_setup(){
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST;         //set UCSWRST;
    //Initialize all eUSCI registers with UCSWRST = 1 (including UCxCTL1).

    //initialize uUSCI_B0 registers keep SWRST set
    EUSCI_B0->CTLW0 =  EUSCI_B_CTLW0_SWRST
    | EUSCI_B_CTLW0_TXACK
    | EUSCI_B_CTLW0_MODE_3 //I2C Mode
    | EUSCI_B_CTLW0_MST //set as Master
    | EUSCI_B_CTLW0_SSEL__ACLK //select SMCLK
    | EUSCI_B_CTLW0_SYNC;       //synchronous mode enable
    EUSCI_B0->CTLW1 &=  ~EUSCI_B_CTLW1_SWACK;

    EUSCI_B0->BRW = 0x05;       //clock speed = 128kHz / 400kHz


    P1->SEL0 |=  SDA | SCL;    //select SDA, SCL page 139 MSP401 data sheet
    P1->SEL1 &=  ~( SDA | SCL );
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    //clear SWRST
    /*EUSCI_B0->IE = EUSCI_B_IE_RXIE; //enable RX interrupts
    EUSCI_B0->IE = EUSCI_B_IE_TXIE; //enable TX interrupts
    NVIC->ISER[0] = ( 1 << EUSCIB0_IRQn );
    __enable_irq();*/
}

void I2C_slave_addr(uint8_t address_){
    address = address_;
}


void EEPROM_mem_addr(uint16_t mem_addr){
    mem_addr_ = mem_addr;
}

void I2C_Write_Char(uint8_t buffer){
    TRANSMITTER;                //set as transmitter mode
    EUSCI_B0->I2CSA =  address;  //load slave address   |   write
    START_BIT;                      //generate start bit
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
    // convert slave address
    uint8_t highByte =  ( mem_addr_ >> 8 ) & 0xFF;
    uint8_t lowByte = ( mem_addr_ & 0xFF );

    //write memory address
    EUSCI_B0->TXBUF = highByte;
    while ( !(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    CLEAR_TX_FLAG;
    EUSCI_B0->TXBUF = lowByte;
    while ( !(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    CLEAR_TX_FLAG;

    //write data
    EUSCI_B0->TXBUF = buffer;
    while(!(EUSCI_B0 ->IFG & EUSCI_B_IFG_TXIFG0));           //wait for transmit buffer to be empty
    CLEAR_TX_FLAG;
    STOP_BIT;
    delay_ms(5);        //required to delay 5ms before any further operations
}

char I2C_Read_Char(){
    TRANSMITTER;                //set as transmitter mode
    EUSCI_B0->I2CSA = address;  //load slave address
    //convert slave address
    uint8_t highByte =  ( mem_addr_ >> 8 ) & 0xFF;
    uint8_t lowByte = ( mem_addr_ & 0xFF );
    DIS_STOP_BIT;
    START_BIT;                      //generate start bit
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);

    //send memory address
    EUSCI_B0->TXBUF = highByte;
    while(!(EUSCI_B0 ->IFG & EUSCI_B_IFG_TXIFG0));           //wait for transmit buffer to be empty
    CLEAR_TX_FLAG;
    EUSCI_B0->TXBUF = lowByte;
    while(!(EUSCI_B0 ->IFG & EUSCI_B_IFG_TXIFG0));           //wait for transmit buffer to be empty
    CLEAR_TX_FLAG;

    RECEIVER;                //set as receiver mode
    START_BIT;                      //generate start bit

    while( !(EUSCI_B0 ->IFG & EUSCI_B_IFG_RXIFG0) );           //wait for data
    uint8_t data = EUSCI_B0->RXBUF;
    CLEAR_RX_FLAG;
    NACK;           // send not acknowledge
    return data;
}

void EUSCIB0_IRQHandler(){

}
