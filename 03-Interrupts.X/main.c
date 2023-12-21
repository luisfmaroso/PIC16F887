#include <xc.h>
#include <pic16f887.h>

#pragma config WDTE = OFF // turn off watchdog timer so it doesn't mess up the delay

#define _XTAL_FREQ 4000000

void __interrupt() ISR(void) 
{
    static uint8_t interruptCount = 0;
    static uint8_t lastRB = 0xFF;

    if (RBIF == 1) 
    {
        RBIF = 0;

        if (PORTB != lastRB) 
        {
            lastRB = PORTB;
            interruptCount++;

            if (interruptCount == 5) 
            {
                for (int i = 0; i < 8; i++) 
                {
                    PORTD = (1 << i);
                    __delay_ms(125);
                }
                PORTD = 0x00;
                interruptCount = 0;
            }
        }
    }
}

void main() 
{
    ANSELH = 0x00; // configure AN8-AN13 as digital
    TRISB = 0xFF;
    nRBPU = 0; // enable individual port B pull-ups

    // configure external interrupt on PORTB
    INTE = 1;  // enable external interrupt
    RBIE = 1;  // enable PORTB change interrupt
    RBIF = 0;  // clear interrupt flag
    IOCB = 0xFF; // enable interrupt on all PORTB pins

    TRISD = 0x00;
    PORTD = 0x00;
    
    GIE = 1; // enable global interrupts

    while(1) { }
}