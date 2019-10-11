/*
 * millis.c
 *
 *  Created on: Oct 10, 2019
 *      Author: User01
 */


#include "msp.h"
#include "millis.h"
#include "stdint.h"

uint64_t _millis;


uint64_t millis(){
    static i;
    if (!i){
        TIMER_A0->CCR[0] = 33;   //2s @ 32.768kHz
        TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
        //select ACLK | select continuous mode
        TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | TIMER_A_CTL_MC__CONTINUOUS ;
        ISER_SEL(TA0_0_IRQn);       //enable CCR0 timer A0 interrupt in NVIC
        __enable_irq();     //enable interrupt globally
        i++;
    }
        return _millis;
}

void TA0_0_IRQHandler(){
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
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;   //clear interrupt flag
    _millis++;
    TIMER_A0->CCR[0] += 33;  //add 0.1 ms
}
