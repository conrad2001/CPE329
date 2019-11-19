/*
 * UART.c
 *
 *  Created on: Oct 24, 2019
 *      Author: User01
 */



#include "msp.h"
#include "UART.h"
#include "set_DCO.h"
#include "SPI.h"

uint16_t data;
char* string;

/*eUCSI_A0->UART
 *initializing eUSCI module 25.3.1 TR manual*/






void EUSCI_A0_UART_setup(){
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;         //set UCSWRST;
    //Initialize all eUSCI registers with UCSWRST = 1 (including UCxCTL1).
    //clear_EUSCI_A0_reg();
    //initialize uUSCI_A0 registers keep SWRST set

    EUSCI_A0->CTLW0 =  EUSCI_A_CTLW0_SWRST
    //| EUSCI_A_CTLW0_PEN     /*!< Parity enable */
    //| EUSCI_A_CTLW0_PAR     /*!< Parity select = 1 */
    //| EUSCI_A_CTLW0_MSB
    | EUSCI_A_CTLW0_MODE_0 //UART Mode
    | EUSCI_A_CTLW0_MST //set as Master
    | EUSCI_A_CTLW0_SSEL__SMCLK; //select SMCLK
    //set falling or rising edge
    EUSCI_A0->BRW = UCBRS;       //divider = 6
    //Modulation Control
    EUSCI_A0-> MCTLW =  ( UCBRSx << 8 )
    | ( UCBRFx << 4)
    | UCOS16 ;
    P1->SEL0 |=  A0_RX | A0_TX;    //select TX, RX page 139 MSP401 data sheet
    P1->SEL1 &=  ~( A0_RX | A0_TX );
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    //clear SWRST
    /*EUSCI_A0->IE = EUSCI_A_IE_RXIE; //enable RX interrupts
    EUSCI_A0->IE = EUSCI_A_IE_TXIE; //enable TX interrupts
    NVIC->ISER[0] = ( 1 << EUSCIA0_IRQn );
    __enable_irq();*/
}

//print out input value and store to data if space character is being input
void EUSCIA0_IRQHandler(){
       static volatile uint16_t num;
       volatile uint16_t buffer = EUSCI_A0->RXBUF & 0xFF;     //reading RXBUF clears RXIFG
       switch(buffer){
       case '0' ... '9':
           num = num * 10 +  ( buffer - '0' );
           EUSCI_A0->TXBUF = buffer ;     //writing TXBUF clears TXIFG
           while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
           break;
       case ' ':
              data = ( num < 4096 ) ? num : data;
              num = 0;
              break;
       /*default:
           *string++ = buffer;
           while (*string){
               EUSCI_A0->TXBUF = *string++ ;     //writing TXBUF clears TXIFG
               while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
           }
           break;*/
       }
}

//return serial DAC value from computer
uint16_t return_DAC_value(){
    return data;
}


/*void Read_UART(char* _buffer, int length){


        //while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_RXIFG));
        //_buffer = buffer;
        *_buffer = EUSCI_A0->RXBUF;

}

void Write_UART(char* _buffer){
    while (*_buffer){
        EUSCI_A0->TXBUF = *_buffer++ ;     //writing TXBUF clears TXIFG
        while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
    }

}*/

