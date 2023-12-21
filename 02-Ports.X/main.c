#include <xc.h>
#include <pic16f887.h>

#pragma config WDTE = OFF // turn off watchdog timer so it doesn't mess up the delay

#define _XTAL_FREQ 4000000

void main() 
{
    ANSEL = 0; // PORTA as digital
    TRISA = 0xFF;
    TRISD = 0x00;
    PORTD = 0x00;

    while (1) 
    {
        switch(PORTA)
        {
            case 0x00:
                PORTD = 0x00;
                break;
            case 0x01:
                __delay_ms(100);
                PORTD = PORTD << 1;
                if (PORTD == 0x00)
                {
                    PORTD = 0x01;
                }
                break;
            case 0x02:
                __delay_ms(100);
                PORTD = PORTD >> 1;
                if (PORTD == 0x00)
                {
                    PORTD = 0x80;
                }
                break;
            case 0x03:
                break;
        }
    }
}