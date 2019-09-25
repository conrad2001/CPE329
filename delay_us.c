/*
 * delay_us.c
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */




#include "msp.h"
#include "delay_us.h"
#include "set_DCO.h"

uint32_t i;


void delay_us(){
    for (i = delay_time; i > 0; i--);        // Delay
}


void set_delay(uint32_t *delay_ptr){
    switch (CS->CTL0){
    //all multipliers for delay_ptr are obtained by calibration
            case FREQ_1p5_MHz:          //MACRO of FREQ_1p5 in set_DCO.h
                *delay_ptr *= (*delay_ptr > 50 ) ? 0.09412 : (0.09412 * 0.788);
                break;
            case FREQ_3_MHz:
                *delay_ptr *= (*delay_ptr > 200 ) ? 0.18827 : (0.18827 * 0.9508);
                break;
            case FREQ_6_MHz:
                *delay_ptr *= (*delay_ptr > 200 ) ? 0.354624223 : (0.354624223 * 0.9508);
                break;
            case FREQ_12_MHz:
                *delay_ptr *= 0.7096160188;
                break;
            case FREQ_24_MHz:
                *delay_ptr *= 1.418697311;
                break;
            case FREQ_48_MHz:
                *delay_ptr *= 2.184335;
                break;
        }
}

