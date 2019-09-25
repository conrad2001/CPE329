/*
 * set_DCO.c
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */


#include "msp.h"
#include "set_DCO.h"


void set_DCO(uint32_t SEL_DCO_FREQ){
    CS->KEY = CS_KEY_VAL;           // Unlock CS module for register access
    if (SEL_DCO_FREQ == CS_CTL0_DCORSEL_5){
        set_Vcore();
        set_flash_controller();
        /* Select MCLK = DCO, no divider */
        CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) | CS_CTL1_SELM_3;
    }
    CS->CTL0 = 0;                    // clear register CTL0
    CS->CTL0 = SEL_DCO_FREQ;        /* select DCO center frequency */
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;    // select clock sources
    CS->KEY = 0;                    // Lock CS module from unintended accesses
}

void MCLK_out(){
    P4->DIR |= BIT3;    //set direction = out
    //select module 1 (MCLK)
    P4->SEL0 |= BIT3;
    P4->SEL1 &= ~BIT3;
}

/*Code for setting Vcore to Level 1 for 48 MHz operation*/
void set_Vcore(){
    /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
     PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
}

/*Code for setting flash controller wait states for 48 MHz operation*/
void set_flash_controller(){
    /* Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
}





/*CS->CTL1 &= ~(CS_CTL1_SELM_MASK | CS_CTL1_SELA_MASK | CS_CTL1_SELS_MASK);
    CS->CTL1 |= CS_CTL1_SELM__DCOCLK | CS_CTL1_SELA__LFXTCLK | CS_CTL1_SELS__DCOCLK;
    CS->CTL1 &= ~(CS_CTL1_DIVM_MASK | CS_CTL1_DIVA_MASK | CS_CTL1_DIVS_MASK);*/
