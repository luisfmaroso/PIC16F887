#include <xc.h>

#pragma config WDTE = OFF

#define _XTAL_FREQ 4000000

void initializeADC() 
{
    VCFG0 = 0; // Vref- is VSS
    VCFG1 = 0; // Vref+ is VDD
    ADFM = 1; // justify to the right
    ADIE = 0; // disable ADC interrupt
    ADCON0bits.ADON = 1; // enable ADC
}

void startADConversion(uint8_t channel) 
{
    ADCON0bits.CHS = channel;
    GO = 1; // start conversion
}

void main() 
{
    TRISD = 0xF0;
    TRISA = 0x01;

    initializeADC();

    while (1) 
    {
        startADConversion(0);

        while (GO); // wait for the conversion

        uint16_t resultChannel0 = (ADRESH << 8) | ADRESL;
        
        if (resultChannel0 < 204) 
        {
            RD0 = 0;
            RD1 = 0;
            RD2 = 0;
            RD3 = 0;
        } 
        else if (resultChannel0 >= 204 && resultChannel0 < 408) 
        {
            RD0 = 1;
            RD1 = 0;
            RD2 = 0;
            RD3 = 0;
        } 
        else if(resultChannel0 >= 408 && resultChannel0 < 612)
        {
            RD0 = 1;
            RD1 = 1;
            RD2 = 0;
            RD3 = 0;
        } 
        else if (resultChannel0 >= 612 && resultChannel0 < 816)
        {
            RD0 = 1;
            RD1 = 1;
            RD2 = 1;
            RD3 = 0;
        }
        else if(resultChannel0 >= 816)
        {
            RD0 = 1;
            RD1 = 1;
            RD2 = 1;
            RD3 = 1;
        }

        __delay_ms(100);
    }
}