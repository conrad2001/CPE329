/*
 * Timer_A0.c
 *
 *  Created on: Oct 7, 2019
 *      Author: User01
 */

#include "msp.h"
#include "ISR.h"
#include "LED_blinky.h"
#include "set_DCO.h"
#include "ISR.h"
#include "CPE329.h"




void timerA0_setup(){
    set_DCO(FREQ_6_MHz);
    TIMER_A0->CCR[0] = 483;   //add 1 clock cycle  @ 12MHz/ 50Hz
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
    //select ACLK | select continuous mode
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;
    ISER_SEL(TA0_0_IRQn);       //enable CCR0 timer A0 interrupt in NVIC

    __enable_irq();     //enable interrupt globally
}

void TA0_0_IRQHandler(){
    static volatile down;
    TIMER_A0->CCR[0] += 483;    //add 1 clock cycle  @ 12MHz/ 50Hz
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    switch(down){
        case 0:
            voltage_bit += 10;
            if (voltage_bit >= 2482 )
                down = 1;
            break;
        case 1:
            voltage_bit -= 10;
            if (voltage_bit <= 0)
                down = 0;
            break;
    }
}

