/*
 * SPI.c
 *
 *  Created on: Oct 16, 2019
 *      Author: User01
 *  Description: connect DAC thorugh SPI and set DAC output voltage
 */


#include "msp.h"
#include "SPI.h"









/*send DAC voltage packets to DAC module*/
void DAC_output(uint16_t outVolt){
    static uint8_t i, hiByte, loByte;
    static uint16_t volt_temp;
    //configure DAC output voltage only under changes of output voltage
    if (!i){
        EUSCI_B0_setup();           //eUCSI_A0->SPI
        P10->DIR |= DAC_CS;
        i++;
    }
    if (outVolt != volt_temp){
        /*covert DAC output voltage to bit*/
        //uint16_t outVolt = voltage / 3.3 * ( 1 << 12 );
        loByte = outVolt & 0xFF;        //truncate to 8 bit
        hiByte = ( outVolt >> 8 ) & 0x0F;       //only want lower 4 bits
        hiByte |= ( GAIN | SHDN );

        P10->OUT &= ~DAC_CS;

        EUSCI_B0 ->TXBUF = hiByte;      //send out the upper 8 bits by MSB
        while (!(EUSCI_B0 ->IFG & EUSCI_B_IFG_TXIFG));
        //wait for TXBUF to be empty
        EUSCI_B0-> TXBUF = loByte;      //send out the lower 8 bits
        while(!(EUSCI_B0 ->IFG & EUSCI_B_IFG_RXIFG));
        //wait for transmit to finish
        P10->OUT |= DAC_CS;
        volt_temp = outVolt;
    }
}





/*eUCSI_A0->SPI
 *initializing eUSCI module 25.3.1 TR manual*/
void EUSCI_B0_setup(){
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST;         //set UCSWRST;
    //Initialize all eUSCI registers with UCSWRST = 1 (including UCxCTL1).
    //clear_EUSCI_B0_reg();
    //initialize uUSCI_A0 registers keep SWRST set
    EUSCI_B0->CTLW0 =  EUSCI_B_CTLW0_SWRST
    //| EUSCI_A_CTLW0_CKPL
    | EUSCI_B_CTLW0_CKPH   //mode 01, active rising edge
    | EUSCI_B_CTLW0_MSB
    | EUSCI_B_CTLW0_MST //set as Master
    | EUSCI_B_CTLW0_SYNC    //synchronous mode
    | EUSCI_B_CTLW0_SSEL__SMCLK; //select SMCLK
    //set falling or rising edge
    EUSCI_B0->BRW = 0x01;       //run full SMCLK speed
    P1->SEL0 |=  SPI_CLK | SPI_MOSI | SPI_MISO;    //select SIMO, SOMI, SCLK page 139 MSP401 data sheet
    P1->SEL1 &=  ~( SPI_CLK | SPI_MOSI | SPI_MISO );
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    //clear SWRST
    /*EUSCI_B0->IE = EUSCI_A_IE_RXIE; //enable RX interrupts
    //EUSCI_B0->IE = EUSCI_A_IE_TXIE; //enable TX interrupts
    NVIC->ISER[0] = ( 1 << EUSCIA0_IRQn );
    __enable_irq();
    EUSCI_B0->TXBUF = 0xAA; //transmit junk*/
}



void EUSCIA0_IRQHandler(){
    EUSCI_B0->RXBUF;     //reading RXBUF clears RXIFG
}

void clear_EUSCI_B0_reg(){
    EUSCI_B0->BRW = 0;
    EUSCI_B0->CTLW1 = 0;
    EUSCI_B0->IE = 0;
    EUSCI_B0->IFG = 0;
    EUSCI_B0->RESERVED0 = 0;
    EUSCI_B0->STATW = 0;
    EUSCI_B0->TXBUF = 0;
}



