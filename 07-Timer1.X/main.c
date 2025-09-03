#include <xc.h>
#include <pic16f887.h>

#pragma config WDTE = OFF // turn off watchdog timer so it doesn't mess up the delay

#define _XTAL_FREQ 4000000

volatile unsigned char stop_generator = 0;

void __interrupt() ISR(void)
{
    stop_generator = 1;
}

void main(void)
{
    TRISD = 0x0F;  // RD0-RD3 input, RD4-RD7 output
    TRISB0 = 0xFF; 
    
    INTE = 1;  // enable external interrupt
    RBIE = 1;  // enable PORTB change interrupt
    RBIF = 0;  // clear interrupt flag
    IOCB = 0xFF; // enable interrupt on all PORTB pins
    
    // Timer1: 1MHz clock (Fosc/4), prescaler 1:8 = 125KHz
    T1CON = 0x30;  // Prescaler 1:8, Timer1 ON bit controlled separately
    
    while(!RD0);   // Wait for start signal
    
    TMR1L = 0xFE;  // Load Timer1 low byte (65534)
    TMR1H = 0xFF;  // Load Timer1 high byte (2 counts to overflow)
    TMR1ON = 1;    // Start Timer1
    
    GIE = 1; // enable global interrupts
    
    while(!stop_generator)  // Run until external interrupt stops it
    {
        if(TMR1IF)  // Check Timer1 overflow (non-blocking)
        {
            TMR1IF = 0;      // Clear overflow flag
            TMR1L = 0xFE;    // Reload timer values
            TMR1H = 0xFF;    // 2 counts = 16us at 125KHz
            RD4 = ~RD4;      // Toggle output for 50% duty cycle
        }
    }
    
    TMR1ON = 0;  // Stop Timer1 when generator is stopped
    RD4 = 0;     // Clear output
}