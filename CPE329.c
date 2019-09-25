/*
 * CPE329.c
 *
 *  Created on: Sep 23, 2019
 *      Author: User01
 */


#include "msp.h"
#include "CPE329.h"

/**
 * main.c
 */


void Assignment_2()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    LED_SETUP();
    set_DCO(FREQ_48_MHz);
    MCLK_out();                                 //enable P4.3 (MCLK) output
    while(1){
            P1->OUT ^= BIT0;                    // XOR P1.0 (turn LED on and off)
            delay_us(50000);                    //delay up to 50000 microseconds
    }

}
