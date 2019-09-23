/*
 * delay_us.c
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */


#include "msp.h"
#include "delay_us.h"

uint32_t i;

void delay_us(uint32_t delay){
    switch (CS->CTL0){
        case CS_CTL0_DCORSEL_0:
            delay *= 0.15;
            break;
        case CS_CTL0_DCORSEL_1:
            delay *= 0.3;
            break;
        case CS_CTL0_DCORSEL_2:
            delay *= 0.6;
            break;
        case CS_CTL0_DCORSEL_3:
            delay *= 1.2;
            break;
        case CS_CTL0_DCORSEL_4:
            delay *= 2.4;
            break;
        case CS_CTL0_DCORSEL_5:
            delay *= 4.8;
            break;
    }
    for (i = delay; i > 0; i--);        // Delay
}
