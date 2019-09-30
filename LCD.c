/*
 * LCD.c
 *
 *  Created on: Sep 24, 2019
 *      Author: User01
 */
#include "msp.h"
#include "LCD.h"
#include "delay_us.h"

/* Instruction Description from page 18 http://www.newhavendisplay.com/app_notes/ST7066U.pdf*/
void Clear_LCD(){
    CLEAR_DISPLAY;
    delay_us(2000);   //minimum delay 1.52ms
}

void Home_LCD(){
    RETURN_HOME;
    delay_us(2000);    //minimum delay 1.52ms
}

void command(uint8_t cmd){
    check_busy();
    WRITE_ENABLE;
    RS_0;           //INSTRUCTION_REGISTER_ENABLE
    P7->OUT = cmd;
    write_trigger();
}

void read_trigger(){
    ENABLE_LOW;         //give enable trigger at rising edge
    delay_us(100);     //minimum delay 80us
    ENABLE_HIGH;
}

void write_trigger(){
    ENABLE_HIGH;       //give enable trigger at falling edge
    delay_us(100);     //minimum delay 80us
    ENABLE_LOW;
}

void Write_char_LCD(uint8_t c){
    check_busy();
    RS_1;
    WRITE_ENABLE;
    P7->OUT = c;
    write_trigger();
}

void Write_string_LCD(char* s){
    while(*s){
        Write_char_LCD(*s);
        s++;
    }
}

void check_busy(){
    P7->DIR &= ~BIT7;      //change P10.7 to input mode
    READ_ENABLE;
    while(BUSY_FLAG){
        read_trigger();       //constantly set and reset enable to check for busy flag
    }
    P7->DIR |= BIT7;      //change P10.7 back to output mode
}

void Init_LCD(){
    P10->DIR |= 0xFF;
    P7->DIR |= 0xFF;
    P10->SEL0 &= 0x00;
    P10->SEL1 &= 0x00;
    P7->SEL0 &= 0x00;
    P7->SEL1 &= 0x00;
    P10->OUT &= 0x00;
    P7->OUT &= 0x00;
    ENABLE_LOW;
    delay_us(50000);    //Wait >40 msec after power is applied
    WAKE_UP;            //wake up
    delay_us(5000);    //must wait 5ms, busy flag not available
    WAKE_UP;            //wake up #2
    delay_us(200);      //must wait 160us, busy flag not available
    WAKE_UP;            //wake up #3
    delay_us(200);      //must wait 160us, busy flag not available
    FUNCTON_SET_8BITS;  //Function set: 8-bit/2-line
    command((uint8_t) BIT4);   //set cursor moving & left shift
    DISPLAY_ON;
    ENTRY_MODE_SET;
    Clear_LCD();
    Home_LCD();
}

void set_cursor(uint8_t column, uint8_t row){
    row = (row < 2) ? ( row << 6 ) : 0;
    uint8_t DDRAM_addr = row | column;
    command((uint8_t) BIT7 | DDRAM_addr); //enable bit 7 and pass in DDRAM address to set cursor position

}
/*E = P10.0 DB7 = P7.7 DB3 = P7.3
RS = P10.1 DB6 = P7.6 DB2 = P7.2
RW = P10.2 DB5 = P7.5 DB1 = P7.1
 DB4 = P7.4 DB0 = P7.0*/
