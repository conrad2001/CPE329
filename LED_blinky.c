/*
 * LED_blnky.c
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */


#include "msp.h"
#include "LED_blinky.h"



void LED_SETUP(){
//select PyOUT.x in MUX
P1->SEL0 &= ~BIT0;  //enable bit 0 in SEL0
P1->SEL1 &= ~BIT0;  //enable bit 0 in SEL1

//set LED output
P1->DIR |= BIT0;    //set direction = output
//P1->OUT |= BIT0;    //enable P1.0 (LED)
}
