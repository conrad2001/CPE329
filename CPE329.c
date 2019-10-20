/*
 * CPE329.c
 *
 *  Created on: Sep 23, 2019
 *      Author: User01
 */


#include "msp.h"
#include "CPE329.h"

/**
 * main.c
 */
/*function generator*/
void Project_2(){
    #define SQUARE 0
    #define SINE 1
    #define SAWTOOTH 2
    /*P10.3 = DAC_CS    P1.5 = SCL   P1.7 = SOMI     P1.6 = SIMO*/
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    FuncGen_setup(SAWTOOTH, 13, 500);
    while(1) DAC_output(voltage_bit);
}


void Assignment_7(){
    /*P10.3 = DAC_CS    P1.5 = SCL   P1.7 = SOMI     P1.6 = SIMO*/
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    timerA0_setup();
    while(1) DAC_output(voltage_bit);
}

void Assignment_5(){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
      /* set P4.3 high and low
      */
    /*partA*/
     Init_LCD();
     LED_SETUP();
     set_DCO(FREQ_1p5_MHz);   //set MCLK, SMCLK = 24MHz
     MCLK_out();
     //TIMER_A0->CCR[0] = 0;
     //TIMER_A0->CCR[1] = 65534;   //
     //TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
     //TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
     //select SMCLK | select up mode
     TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | TIMER_A_CTL_MC__CONTINUOUS ;
     //ISER_SEL(TA0_0_IRQn);       //enable CCR0 timer A0 interrupt in NVIC
     //ISER_SEL(TA0_N_IRQn);       //enable CCR0 timer A1~4 interrupt in NVIC
     enable_ISR(PORT1_IRQn);    //enable PORT1 interrupt in NVIC
     //set LED output
     __enable_irq();     //enable interrupt globally
     while(1);
}


void Assignment_4(){
    char key;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(FREQ_1p5_MHz);
    Init_LCD();
    while(1){
        key = scan_key();
        if(key){
            Write_char_LCD(key);
            delay_ms(200);
            continue;
        }
    }
}

void Assignment_3(){        //clear, home, print LCD
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(FREQ_12_MHz);
    Init_LCD();     //initialize LCD
    Clear_LCD();
    while(1){
        Japanese_LCD_GIF();
    }
}



void Assignment_2()     //delay LED for 40ms
{
    #include "delay_us.h"
    //set delay time up to 50000 microsecond
    delay_time = 50000;                             //variable "delay_time" declared in delay_us.h
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    LED_SETUP();
    set_DCO(FREQ_1p5_MHz);
    MCLK_out();                                 //enable P4.3 (MCLK) output
    set_delay(&delay_time);                     //setup delay time pass by reference
    while(1){
            P1->OUT ^= BIT0;                    // XOR P1.0 (turn LED on and off)
            run_delay_us();
    }

}


void Timer_class_example(){
    /*blink LED0 0.5Hz CCR[0]
     * blink LED1 1Hz CCR[1]
     * -ACLK 32.678kHz
     *
     */
    LED_SETUP();
    TIMER_A0->CCR[1] = 32767;   //1S @ 32.768kHz
    TIMER_A0->CCR[0] = 65535;   //2s @ 32.768kHz
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt, compare mode
    //select ACLK | select continuous mode
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | TIMER_A_CTL_MC__CONTINUOUS;
    ISER_SEL(TA0_0_IRQn);       //enable CCR0 timer A0 interrupt in NVIC
    ISER_SEL(TA0_N_IRQn);       //enable CCR1 timer A0 interrupt in NVIC
    __enable_irq();     //enable interrupt globally
    while(1);
}

void Japanese_LCD_GIF(){
    Clear_LCD();
    Home_LCD();
    Write_string_LCD("(*^^)(^^*)");
    delay_s(1);
    Clear_LCD();
    Home_LCD();
    Write_string_LCD("(*^^)^^*)");
    char buf[] = {j_CHI, j_yu, j_tsu};
    Write_string_LCD(buf);
    set_cursor(0,1);
    char buf2[] = {j_SHI, j_A, j_WA, j_SE, '!'};
    Write_string_LCD(buf2);
    delay_s(1);
}

void test_int_char(){
    char key[6];
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(FREQ_1p5_MHz);
    Init_LCD();
    sprintf(key, "%d", 135765);
    Write_string_LCD(key);
}

