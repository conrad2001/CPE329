/*
 * ADC.h
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

#ifndef ADC_H_
#define ADC_H_

void ADC_SETUP();
uint16_t return_ADC_value();

#endif /* ADC_H_ */
