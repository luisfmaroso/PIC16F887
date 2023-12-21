#include <xc.h>

#pragma config WDTE = OFF

#define _XTAL_FREQ 4000000

void writeEEPROM(uint16_t address, uint16_t data) 
{
    GIE = 0;

    EEADR = address;
    EEDATA = data;

    EEPGD = 0; // EEPROM data access
    WREN = 1;  // enable EEPROM write

    // write sequence
    GIE = 0;       // disable interruptions
    EECON2 = 0x55; // mandatory sequency
    EECON2 = 0xAA; // mandatory sequency
    WR = 1;        // start write

    while (WR); // wait for write

    WREN = 0; // disable write
    GIE = 1; // enable interrupts
}

void main() 
{
    uint16_t fib[10];
    fib[0] = 1;
    writeEEPROM(0, fib[0]);
    fib[1] = 1;
    writeEEPROM(1, fib[1]);

    for (uint8_t i = 2; i < 10; i++) 
    {
        fib[i] = fib[i - 1] + fib[i - 2];
       writeEEPROM(i, fib[i]);
    }
   
    while (1) { }
}