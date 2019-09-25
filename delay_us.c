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
            delay *= 0.0470675;
            break;
        case CS_CTL0_DCORSEL_1:
            delay *= 0.09412;
            break;
        case CS_CTL0_DCORSEL_2:
            delay *= 0.18827;
            break;
        case CS_CTL0_DCORSEL_3:
            delay *= 0.37654;
            break;
        case CS_CTL0_DCORSEL_4:
            delay *= 0.75308;
            break;
        case CS_CTL0_DCORSEL_5:
            delay *= 1.14965;
            break;
    }
    for (i = delay; i > 0; i--);        // Delay
}
