/*
 * Function_Generator.c
 *
 *  Created on: Oct 19, 2019
 *      Author: User01
 */


#include "msp.h"
#include "ISR.h"
#include "LED_blinky.h"
#include "set_DCO.h"
#include "ISR.h"
#include "CPE329.h"
#include <math.h>

enum waveform{
    square_wave,
    sine_wave,
    sawtooth_wave
};

uint8_t _waveform, _DutyCycle;
uint16_t sine_bit[314], _CCR_bit;

void FuncGen_setup(uint8_t waveform, uint8_t DutyCycle, uint16_t freq){
    set_DCO(FREQ_24_MHz);
    _waveform = waveform;
    _DutyCycle = DutyCycle;

    uint16_t i;
    TIMER_A1->CCR[0] = 0;   //add 1 clock cycle  @ 12MHz/ 100Hz
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, Toggle/reset mode
    //select SMCLK | select continuous mode
    TIMER_A1->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;
    ISER_SEL(TA1_0_IRQn);       //enable CCR0 timer A1 interrupt in NVIC
    TIMER_A1->CCR[0] = 0;   //add 1 clock cycle  @ 12MHz/ 100Hz
    switch(waveform){
    case square_wave:
        //adjust bit based on freq
        _CCR_bit = ( 2380 * 100 / freq );
        break;
    case sine_wave:
        //adjust bit based on freq
        _CCR_bit = ( 765 * 100 / freq );
        //load in values of sine into an array
        for ( i = 0; i < 314 ; i++)
           sine_bit[i] = ( 1.5 * sin( (float) i / 50 ) + 1.5 ) / 3.3 * ( 1 << 12) ;
        break;
    case sawtooth_wave:
        //adjust bit based on freq
        _CCR_bit = ( 967 * 100 / freq );
        break;
    }

    __enable_irq();     //enable interrupt globally
}


void TA1_0_IRQHandler(){
    static volatile down;
    static uint16_t i;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    switch(_waveform){
        case square_wave:
            TIMER_A1->CCR[0] += _CCR_bit;    //add 1 clock cycle  @ 12MHz/ 100Hz
            // cut into 100 pieces, enable the number of pieces high depending on the Duty Cycle
            if ( i < _DutyCycle ){
                voltage_bit = 3760;
                i++;
            }
            // the rest are low
            else if ( i < 100 ){
                voltage_bit = 0;
                i++;
            }
            //repeat the counter
            else i = 0;
            break;
        case sine_wave:
            TIMER_A1->CCR[0] += _CCR_bit ;    //add 1 clock cycle  @ 12MHz/ 100Hz
            if( i < 314 )
                //unload values from sine array
                voltage_bit = sine_bit[i++];
            else i = 0;
            break;
        case sawtooth_wave:
            TIMER_A1->CCR[0] += _CCR_bit;    //add 1 clock cycle  @ 12MHz/ 100Hz
            //increment voltage by 8.056mV
            voltage_bit += 10;
            if (voltage_bit >= 3760 )
                //reset voltage to 0 if exceeds 3V
                voltage_bit = 0;
        }
}

