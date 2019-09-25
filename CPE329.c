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
    #include "delay_us.h"
    //set delay time up to 50000 microsecond
    delay_time = 40;                             //variable "delay_time" declared in delay_us.h
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    LED_SETUP();
    set_DCO(FREQ_1p5_MHz);
    MCLK_out();                                 //enable P4.3 (MCLK) output
    set_delay(&delay_time);                     //setup delay time pass by reference
    while(1){
            P1->OUT ^= BIT0;                    // XOR P1.0 (turn LED on and off)
            delay_us();
    }

}
