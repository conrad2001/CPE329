/*
 * ISR.h
 *
 *  Created on: Oct 6, 2019
 *      Author: User01
 */

#ifndef ISR_H_
#define ISR_H_


void enable_ISR(uint8_t port);
void ISER_SEL(uint8_t port);
void PORT1_IRQHandler();
void TA0_0_IRQHandler();
void TA0_N_IRQHandler();

#endif /* ISR_H_ */
