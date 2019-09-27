/*
 * delay_us.h
 *
 *  Created on: Sep 21, 2019
 *      Author: User01
 */

#ifndef DELAY_US_H_
#define DELAY_US_H_

uint32_t delay_time;

void delay_s(uint32_t delay);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay_ptr);
void run_delay_us();
void set_delay(uint32_t *delay_ptr);

#endif /* DELAY_US_H_ */
