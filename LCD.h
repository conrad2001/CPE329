/*
 * LCD.h
 *
 *  Created on: Sep 24, 2019
 *      Author: User01
 */

#ifndef LCD_H_
#define LCD_H_
/* ENABLE (E) CONFIGURATION*/
#define ENABLE_HIGH                     P10->OUT |=  BIT0
#define ENABLE_LOW                      P10->OUT &= ~BIT0
/* Register select (RS) CONFIGURATION*/
#define RS_1                            P10->OUT |=  BIT1        //DATA_REGISTER_ENABLE
#define RS_0                            P10->OUT &= ~BIT1        //INSTRUCTION_REGISTER_ENABLE
/* Read/Write (RW) CONFIGURATION*/
#define READ_ENABLE                     P10->OUT |=  BIT2        //RW = 1
#define WRITE_ENABLE                    P10->OUT &= ~BIT2        //RW = 0


/*Table of Commands*/
/*refer to page 17 of http://www.newhavendisplay.com/app_notes/ST7066U.pdf*/
/* or page 6 of https://www.newhavendisplay.com/specs/NHD-0216HZ-FSW-FBW-33V3C.pdf */
#define CLEAR_DISPLAY           command(0x01)    /*Write “20H” to DDRAM and set DDRAM address to “00H” from AC*/
#define RETURN_HOME             command(0x02)    /*Set DDRAM Address to “00H” from AC and return cursor to its original position if shifted. The contents of DDRAM are not changed.*/
//#define ENTRY_MODE_SET
#define DISPLAY_ON              command(0x0F)    /*0x00001DCB D=1:entire display on C=1:cursor on B=1:cursor position on*/
//#define CURSOR_SHIFT
#define FUNCTON_SET_8BITS       command(0x38)   /*BIT4:interface data is 8/4 bits BIT3:number of line is 2/1 BIT2:font size is 5x11/5x8*/
#define FUNCTON_SET_4BITS       command(0x28)
#define ENTRY_MODE_SET          command(0x06)
#define BUSY_FLAG               P7->IN & BIT7 /*BIT 7 HIGH = BUSY, LOW = FREE*/
#define WAKE_UP                 command(0x30)






void Clear_LCD();   //clear the display
void Home_LCD();    // move the cursor to the top left of the LCD
void command(uint8_t cmd);
void read_trigger();        //trigger read
void write_trigger();        //trigger write
void Write_char_LCD(uint8_t c);
void Write_string_LCD(char* s);
void check_busy();
void Init_LCD();        //initialize LCD
void set_cursor(uint8_t column, uint8_t row);      //set cursor position
void lcd_display_char(unsigned char number);
void convert_int_char(int input_integer);
#endif /* LCD_H_ */


/*E = P10.0 DB7 = P7.7 DB3 = P7.3
RS = P10.1 DB6 = P7.6 DB2 = P7.2
RW = P10.2 DB5 = P7.5 DB1 = P7.1
 DB4 = P7.4 DB0 = P7.0*/
