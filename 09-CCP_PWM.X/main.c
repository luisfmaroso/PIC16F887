#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ 4000000UL
#include <xc.h>
#include <stdio.h>
#include "display.h"
#include "keypad.h"

unsigned char duty_percent = 0;

void pwm_init() 
{
    TRISCbits.TRISC2 = 0;  // RC2 as output (CCP1)
    
    // configure Timer2 for PWM
    PR2 = 249;
    T2CON = 0x05;  // TMR2 ON, prescaler = 4
    CCP1CON = 0x0C;  // CCP1 PWM mode
    CCPR1L = 0;
    CCP1CONbits.DC1B = 0;
}

void pwm_set_duty(unsigned char percent) 
{
    if (percent > 100) percent = 100;
    
    unsigned int duty = (unsigned int)percent * 10;
    CCPR1L = (unsigned char)(duty >> 2);
    CCP1CONbits.DC1B = (unsigned char)(duty & 0x03);
}

void main() 
{
    OSCCON = 0x61;
    ANSEL = 0; ANSELH = 0;
    pwm_init();
    display_init();
    keypad_init();
    lcd_write(0x80, "PWM: 0%         ");
    lcd_write(0xC0, "Press 0-9...    ");
    
    while (1) 
    {
        int key = keypad_read();
        
        if (key != -1) 
        {
            if (key >= 0 && key <= 9) 
            {
                char buffer[17];
                sprintf(buffer, "Input:%d Press *", key);
                lcd_write(0xC0, buffer);
                
                // wait for key release
                while(keypad_read() != -1) 
                {
                    __delay_ms(10);
                }
                
                // wait for asterisk
                while(1) 
                {
                    __delay_ms(50);
                    
                    int confirm = keypad_read();
                    if(confirm == -6) // asterisk
                    {  
                        duty_percent = key * 10;
                        pwm_set_duty(duty_percent);
                        
                        sprintf(buffer, "PWM: %d%%  ", duty_percent);
                        lcd_write(0x80, buffer);
                        lcd_write(0xC0, "OK! Press 0-9...");
                        
                        while(keypad_read() != -1) 
                        {
                            __delay_ms(10);
                        }
                        
                        break;
                    }
                    else if(confirm >= 0 && confirm <= 9) 
                    {
                        key = confirm;
                        sprintf(buffer, "Input:%d Press *", key);
                        lcd_write(0xC0, buffer);
                        
                        while(keypad_read() != -1) 
                        {
                            __delay_ms(10);
                        }
                    }
                }
            }
            
            while(keypad_read() != -1) 
            {
                __delay_ms(10);
            }
        }
        
        __delay_ms(50);
    }
}