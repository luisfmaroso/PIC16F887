#include <xc.h>
#include <pic16f887.h>

#define _XTAL_FREQ 4000000
#define LED RD0

void main() 
{
    TRISD0 = 0;
    while(1)
    {
        LED = 0;
        __delay_ms(500);
        LED = 1;
        __delay_ms(500);
    }
}