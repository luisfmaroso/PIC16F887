#define _XTAL_FREQ 4000000UL
#include <xc.h>
#include "keypad.h"

void keypad_init(void) 
{
    TRISB = 0xF0;   // RB0-RB3 outputs, RB4-RB7 inputs
    PORTB = 0x0F;   // Set all column selects high initially
    OPTION_REGbits.nRBPU = 0;  // Enable weak pull-ups on PORTB
    WPUB = 0xF0;  // Enable pull-ups on RB4-RB7
}

int keypad_read() 
{
    int key = -1;
    
    PORTB = (PORTB & 0xF0) | 0b1101;  // RB3=1, RB2=1, RB1=0, RB0=X
    __delay_us(50);
    if(!PORTBbits.RB4) { key = 1; }
    else if(!PORTBbits.RB5) { key = 4; }
    else if(!PORTBbits.RB6) { key = 7; }
    else if(!PORTBbits.RB7) { key = -6; }  // *
    
    if(key != -1) return key;
    
    PORTB = (PORTB & 0xF0) | 0b1011;  // RB3=1, RB2=0, RB1=1, RB0=X
    __delay_us(50);
    if(!PORTBbits.RB4) { key = 2; }
    else if(!PORTBbits.RB5) { key = 5; }
    else if(!PORTBbits.RB6) { key = 8; }
    else if(!PORTBbits.RB7) { key = 0; }
    
    if(key != -1) return key;
    
    PORTB = (PORTB & 0xF0) | 0b0111;  // RB3=0, RB2=1, RB1=1, RB0=X
    __delay_us(50);
    if(!PORTBbits.RB4) { key = 3; }
    else if(!PORTBbits.RB5) { key = 6; }
    else if(!PORTBbits.RB6) { key = 9; }
    else if(!PORTBbits.RB7) { key = -7; }  // #
    
    return key;
}