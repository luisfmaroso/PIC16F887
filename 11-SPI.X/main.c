#include <xc.h>
#include <stdint.h>

#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000UL   // 4 MHz internal oscillator

void SPI_Init() 
{
    OSCCON = 0b01100001;  // IRCF=110 -> 4 MHz, SCS=01 internal

    ANSEL = 0x00;
    ANSELH = 0x00;

    // SPI pins
    TRISC3 = 0; // SCK  -> output
    TRISC4 = 1; // SDI  -> input
    TRISC5 = 0; // SDO  -> output

    // SS pin (active low)
    TRISC7 = 0;
    RC7 = 0;

    // SPI setup: Mode 0 (CKP=0, CKE=0)
    SSPSTAT = 0x00;
    SSPCON  = 0x20;  // SSPEN=1, Master mode Fosc/4 (1 MHz clock)

    PIR1bits.SSPIF = 0;  // clear flag
}

void SPI_Write(uint8_t data) 
{
    PIR1bits.SSPIF = 0;
    SSPBUF = data;
    while(!PIR1bits.SSPIF);  // wait for transmission
    PIR1bits.SSPIF = 0;
}

void main() 
{
    uint8_t count = 1;

    SPI_Init();
    __delay_ms(100);

    while (1) 
    {
        SPI_Write(count++);
        if (count == 0) count = 1; // wrap at 255
        __delay_ms(500);
    }
}
