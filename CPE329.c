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
void Assignment_10(){   //I2C
    set_DCO(FREQ_3_MHz);      //set DCO frequency
    CS->KEY = CS_KEY_VAL;
    CS->CLKEN |= CS_CLKEN_REFOFSEL | CS_CLKEN_REFO_EN;  //setup 128kHz for ACLK
    CS->KEY = 0;
    EUSCI_B0_I2C_setup();   //setup I2C
    I2C_slave_addr(0x50);  //set slave address
    EUSCI_A0_UART_setup();
    EEPROM_mem_addr(0x2222);      //point to memory address
    while(1){
        I2C_Write_Char(0x32);        //write character
        char read_char = I2C_Read_Char();
        EUSCI_A0->TXBUF = read_char ;     //writing TXBUF clears TXIFG
        while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
    }
}




void Assignment_9(){    //UART
    const uint8_t decimal_point = 5;
    EUSCI_A0_UART_setup();
    ADC_SETUP();
    uint32_t ADC_value, ADC_new;
    int voltage;
    int power[decimal_point];
    int i;
    for ( i = 0; i < decimal_point; i++){
        power[i] = pow( 10 , decimal_point - i );
    }
    while(1){
        DAC_output(return_DAC_value());
        ADC_value = return_ADC_value();
        if (ADC_value){
            //int i;
            voltage = ADC_value * 3.3 / ( 1 << 14 ) * power[0];
            for ( i = 0; i < decimal_point; i++){
                if ( i == 1 ){
                    EUSCI_A0->TXBUF = '.';
                    while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
                }

                EUSCI_A0->TXBUF = ( voltage / power[i]  ) + '0';       //get and print MSB in decimal
                voltage = voltage  - ((int)(voltage / power[i] ))* power[i];          //take remainder
                while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
            }
            /*for ( i = 3; i >= 0; i--){
                if ( i == 2)
                    EUSCI_A0->TXBUF = '.';
                EUSCI_A0->TXBUF = ( ADC_value / pow( 10 , i ) ) + '0';       //get and print MSB in decimal
                ADC_value = ADC_value  - ((int)(ADC_value / pow( 10 , i ) ))* pow( 10 , i );          //take remainder
                while(!(EUSCI_A0 ->IFG & EUSCI_A_IFG_TXIFG));
           }*/

            EUSCI_A0->TXBUF = ' ';
           delay_ms(100);
           ADC_new = ADC_value;
        }
    }
}


void Assignment_8(){    //UART, DAC output controlled from computer
    EUSCI_A0_UART_setup();
    while(1)   DAC_output(return_DAC_value());
}


/*function generator*/
void Project_2(){     //Function Generator
    char key;
    uint16_t freq = 100;
    uint8_t  wave_type = 1, Duty_Cycle = 50;
    Init_LCD();
    /*P10.3 = DAC_CS    P1.5 = SCL   P1.7 = SOMI     P1.6 = SIMO*/
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(FREQ_24_MHz);

    while(1) {
        key = scan_key();       //scan for key
        if(key){
            switch(key){
            // change frequencies
            case '1' ... '5':
                freq = ( key - '0' ) * 100;
                Clear_LCD();
                Write_char_LCD(key);
                Write_string_LCD("00Hz");
                break;
            // change wave type
            case '6' ... '9':
                Clear_LCD();
                wave_type = ( key - '6' );
                switch(key){
                case '6':   Write_string_LCD("heart");            break;
                case '7':   Write_string_LCD("Square Wave");      break;
                case '8':   Write_string_LCD("Sine Wave");        break;
                case '9':   Write_string_LCD("Sawtooth Wave");    break;
                }
                break;
            // adjust duty cycle
            case '#': Duty_Cycle += ( Duty_Cycle < 90 ) ? 10 : 0; break;
            case '*': Duty_Cycle -= ( Duty_Cycle > 10 ) ? 10 : 0; break;
            // reset duty cycle
            case '0': Duty_Cycle = 50;  break;
            }
            delay_ms(200);
            continue;
        }
        FuncGen_setup(wave_type, Duty_Cycle, freq);
        DAC_output(voltage_bit);
    }
}


void Assignment_7(){    //SPI and DAC
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


void Assignment_4(){    //keypad, print entered key to LCD
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

