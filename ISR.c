/*
 * ISR.c
 *
 *  Created on: Oct 6, 2019
 *      Author: User01
 */

#include "msp.h"
#include "ISR.h"
#include "LED_blinky.h"
#include "LCD.h"
#include "delay_us.h"
#include "CPE329.h"

void enable_ISR(uint8_t port){
    switch(port){
    //enable interrupt on P1.1
    case PORT1_IRQn:
        P1->DIR &= ~( BIT1 | BIT4 );
        P1->REN |=  BIT1 | BIT4;
        P1->OUT |=  BIT1 | BIT4;
        P1->IE  |=  BIT1 | BIT4;    //enable interrupt for P1.1
        P1->IES |=  BIT1 | BIT4;    //enable interrupt P1.1 on falling edge
        P1->IFG &= ~( BIT1 | BIT4 );    //initialize flags
        ISER_SEL(PORT1_IRQn);
        __enable_irq();     //enable global ints
        break;
    }
}

void ISER_SEL(uint8_t port){
    //enable interrupt register in port
    if (port > 31 ) NVIC->ISER[1] = ( 1 << ( port & 31 ) );
    else            NVIC->ISER[0] = ( 1 << ( port & 31 ) );
}

char time_diff_str[6];
volatile uint16_t time_P1[2];
static uint8_t i;

void PORT1_IRQHandler(){
    //clear flag on P1.1
    P1->IFG &= ~( BIT1 | BIT4 );
    //record time
    time_P1[i++] = TIMER_A0->R;
    if (i > 1 ){
        uint16_t time_diff = ((time_P1[1] - time_P1[0] > 0 ) ? time_P1[1] - time_P1[0] : time_P1[1] - time_P1[0] + 65535) / 32.768;
        sprintf(time_diff_str, "%d", time_diff );
        Clear_LCD();
        Write_string_LCD( "time difference is" );
        set_cursor(0,1);
        Write_string_LCD( time_diff_str );
        Write_string_LCD( " ms" );
        i = 0;
        delay_ms(200);

    }

}



void TA0_N_IRQHandler(){        //CCR 1-4 ISR
    /*TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;   //clear interrupt flag
    P1->OUT ^= BIT0;
    P1->OUT ^= BIT0;*/
    //CCR0 ISR, turn LED on and off
    /*static i;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;   //clear interrupt flag
    if( i < 1) {
        P1->OUT &= ~BIT0;
        i++;
    }
    else {
        P1->OUT |= BIT0;
        i = 0;
    }*/
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;   //clear interrupt flag
    TIMER_A0->CCR[1] += 65535;  //add 25% of 25kHz
    PORT1_IRQHandler();
}

