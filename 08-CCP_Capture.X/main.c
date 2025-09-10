#include <xc.h>
#include <string.h>
#include "display.h"

#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

volatile unsigned int period_us = 0;
char text[17];

int main()
{
    unsigned char low, high;

    // disable analog inputs
    ANSEL = 0x00;
    ANSELH = 0x00;

    TRISD = 0x00;
    TRISE = 0x00;
    TRISCbits.TRISC2 = 1;

    PORTD = 0x00;
    PORTE = 0x00;

    lcd_init();
    strcpy(text, " CCP CAPTURE");
    lcd_write(0x80, text);
    strcpy(text, "PER. = 00,00 ms");
    lcd_write(0xC0, text);

    // timer1 Fosc/4, prescaler 1:1
    TMR1CS = 0;
    T1CONbits.T1CKPS = 0b00;
    TMR1H = 0; TMR1L = 0;

    CCP1CON = 0b00000100; // CCP1 capture on falling edge

    while (1)
    {
        PIR1bits.TMR1IF = 0;
        T1CONbits.TMR1ON = 0;
        TMR1H = 0; TMR1L = 0;

        while (!PIR1bits.CCP1IF) { }

        T1CONbits.TMR1ON = 1;
        PIR1bits.CCP1IF = 0;

        while (!PIR1bits.CCP1IF) { }
        PIR1bits.CCP1IF = 0;

        low = CCPR1L;
        high = CCPR1H;
        period_us = (unsigned int)high * 256u + (unsigned int)low;

        convert_period();
        update_display();
    }

    return 0;
}
