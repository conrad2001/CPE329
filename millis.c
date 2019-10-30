/*
 * millis.c
 *
 *  Created on: Oct 10, 2019
 *      Author: User01
 */


#include "msp.h"
#include "millis.h"

uint64_t _millis;


uint64_t millis(){
    static i;
    if (!i){
        //fix ACLK at 32.768kHz
        CS->KEY = CS_KEY_VAL;           // Unlock CS module for register access
        CS->CTL1 &= ~( CS_CTL1_SELA__LFXTCLK | CS_CTL1_SELA__VLOCLK );  //fix ACLK to use REFOCLK
        CS->CTL1 |= CS_CTL1_SELA__REFOCLK;
        CS->KEY = 0;                    // Lock CS module from unintended accesses
        //check ACLK divider and match timing to 1ms for different dividing factors
        switch(CS->CTL1 & 1 << 24){
        case CS_CTL1_DIVA__1:   TIMER_A3->CCR[0]   = 31;   //1ms @ 32.768kHz
        case CS_CTL1_DIVA__2:   TIMER_A3->CCR[0]   = 62;
        case CS_CTL1_DIVA__4:   TIMER_A3->CCR[0]   = 124;
        case CS_CTL1_DIVA__8:   TIMER_A3->CCR[0]   = 248;
        case CS_CTL1_DIVA__16:  TIMER_A3->CCR[0]   = 496;
        case CS_CTL1_DIVA__32:  TIMER_A3->CCR[0]   = 992;
        case CS_CTL1_DIVA__64:  TIMER_A3->CCR[0]   = 1984;
        case CS_CTL1_DIVA__128: TIMER_A3->CCR[0]   = 3968;
        }

        TIMER_A3->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
        //select ACLK | select continuous mode
        TIMER_A3->CTL = TIMER_A_CTL_SSEL__ACLK | TIMER_A_CTL_MC__CONTINUOUS ;
        NVIC->ISER[0] = ( 1 << ( TA3_0_IRQn & 31 ) );   //enable CCR0 timer A3 interrupt in NVIC
        __enable_irq();     //enable interrupt globally
        i++;
    }
        return _millis;
}

void TA3_0_IRQHandler(){
    TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;   //clear interrupt flag
    _millis++;
    TIMER_A3->CCR[0] += 32;  //add 0.1 ms @ 32.768kHz
}


