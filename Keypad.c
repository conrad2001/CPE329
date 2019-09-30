/*
 * Keypad.c
 *
 *  Created on: Sep 29, 2019
 *      Author: User01
 */

#include "msp.h"
#include "Keypad.h"
#include "delay_us.h"



const uint8_t column = 3;
const uint8_t row = 4;
char keypad_matrix[row][column]={{'1', '2', '3'},
                                 {'4', '5', '6'},
                                 {'7', '8', '9'},
                                 {'*', '0', '#'}};




char scan_key(){
    Init_KEYPAD();
    char i, j;
    char row_num;
    while (!row_num){
        for(i = 0; i < column; i++){
            P9->OUT = 0x00;               //clear all column outputs to 0, otherwise multiple outputs would happen
            P9->OUT |= (BIT0 << i);       //loop column 0 to 2 from BIT0 to BIT2
            __delay_cycles(25);
            for(j = 0; j < row; j++){
                row_num = ( P9->IN & (BIT3 << j) );   //loop row 0 to 3 from BIT3 to BIT6
                if(row_num) {
                    __delay_cycles(25);
                    return keypad_matrix[j][i];
                }
            }
        }
        return 0;           //if not return 0, the return value will be all the same
    }
}




void Init_KEYPAD(){
    P9->DIR = BIT0 | BIT1 | BIT2;               //set column 0, 1, 2 to output
    P9->REN = BIT3 | BIT4 | BIT5 | BIT6;        //set row 0, 1, 2, 3 to input
    P9->OUT = 0x00;                             //initialize output of column 0, 1, 2 to be 0
    P9->SEL0 = 0x00;
    P9->SEL1 = 0x00;
}

/*BIT0 = COL0
 *BIT1 = COL1
 *BIT2 = COL2
 *BIT3 = ROW0
 *BIT4 = ROW1
 *BIT5 = ROW2
 *BIT6 = ROW3
 *COL = OUTPUT LOOP
 *ROW = INPUT LOOP
*/
