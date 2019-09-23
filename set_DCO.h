/*
 * set_DCO.h
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */

#ifndef SET_DCO_H_
#define SET_DCO_H_


#define FREQ_1p5_MHz CS_CTL0_DCORSEL_0
#define FREQ_3_MHz   CS_CTL0_DCORSEL_1
#define FREQ_6_MHz   CS_CTL0_DCORSEL_2
#define FREQ_12_MHz  CS_CTL0_DCORSEL_3
#define FREQ_24_MHz  CS_CTL0_DCORSEL_4
#define FREQ_48_MHz  CS_CTL0_DCORSEL_5

void set_DCO(uint32_t SEL_DCO_FREQ);
void MCLK_out();
void set_Vcore();
void set_flash_controller();


#endif /* SET_DCO_H_ */





//refer to http://www.ti.com/lit/an/slaa658a/slaa658a.pdf
//http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/558695?Setting-DCOCLK-to-1048576-on-MSP432
//https://embedds.com/configuring-the-dco-of-msp430/
