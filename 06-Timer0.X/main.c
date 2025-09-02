#include <xc.h>
#include <pic16f887.h>

#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000

void main(void)
{
    unsigned int count = 0;
    
    TRISA = 0x00;
    TRISD = 0x01;
    RA0 = 1;
    RD0 = 0;
    
    // Timer0: Internal clock (Fosc/4 = 1MHz), no prescaler
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0;
    
    TMR0 = 0xCE;     // 50 counts (206 to 256) = 50us per overflow
    
    while(!RD0);
    
    while(1)
    {
        while(!TMR0IF);
        TMR0IF = 0;
        TMR0 = 0xCE;
        count++;
        
        if(count == 3)   // 150us: end of 30% high time (go LOW)
        {
            RA0 = 0;
        }
        
        if(count == 10)  // 500us: complete period, start next cycle (go HIGH)
        {
            RA0 = 1;
            count = 0;
        }
    }
}