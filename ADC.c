/*
 * ADC.c
 *
 *  Created on: Oct 27, 2019
 *      Author: User01
 */


/* ADC 14bit
-14 bit resolution
-analog channel A0 (p5.5)
-save mem[1]
-SMCLK @ no dividers
-sample 4 clocks
-sample repeatedly
*/

#include "msp.h"
#include "ADC.h"

static uint32_t Digital;

/*ADC14_CTL0*/
void ADC_SETUP(){
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; //disable ADC
    ADC14->CTL0 = ADC14_CTL0_SHP    //select internal sample timer
    | ADC14_CTL0_SSEL__SMCLK   //select SMCLK
    | ADC14_CTL0_CONSEQ_2       //repeat single channel
    | ADC14_CTL0_SHT0__4        //sample 4 clocks
    | ADC14_CTL0_MSC            //auto repeat
    | ADC14_CTL0_ON;            //turn on ADC

    /*ADC14_CTL1*/
    ADC14->CTL1 = ( 1 << ADC14_CTL1_CSTARTADD_OFS) // start  at mem[1]
    | ADC14_CTL1_RES__14BIT;        //select 14-bit resolution

    /*ADC14_MCTL[1] memory control register 1*/
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_0;        //select A0 for conversion

    /*ADC14_IER0 Interrupt Enable 0 Register*/
    ADC14->IER0 = ADC14_IER0_IE1;       //enable interrupts on mem[1]

    ADC14->CTL0 |= ADC14_CTL0_ENC;  //enable ADC

    /*configure ports*/
    P5->SEL0 |= BIT5;   //select A0 for P5.5
    P5->SEL1 |= BIT5;

    /*start conversion*/
    ADC14->CTL0 |= ADC14_CTL0_SC;

    NVIC->ISER[0] = ( 1 << ADC14_IRQn);
    __enable_irq();
}


void ADC14_IRQHandler(){
    //ADC14->CLRIFGR0 &= ~ADC14_IFGR0_IFG1;
    Digital = ADC14->MEM[1];
}

uint16_t return_ADC_value(){
    return Digital;
}
