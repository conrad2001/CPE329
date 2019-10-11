#include "CPE329.h"

/**
 * main.c
 */

void main(void)
{
    char key[6];
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    static uint32_t time = 0;
    millis();
    Init_LCD();


    while(1)
    {
        if( millis() - time > 1000){
            time = millis();
            Clear_LCD();
            sprintf(key, "%d ", time);
            Write_string_LCD(key);
        }
    }
}
