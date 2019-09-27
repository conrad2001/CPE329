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
