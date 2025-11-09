#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000UL

#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config LVP  = OFF

#define I2C_FREQ_DIV 9   // SSPADD for Fosc=4MHz, Fscl=100k

void I2C_Init() 
{
    OSCCON = 0x61;          // internal osc @4MHz
    ANSEL = 0; ANSELH = 0;
    TRISC3 = 1; TRISC4 = 1; // SCL, SDA as inputs (peripheral controls pins)
    SSPADD = I2C_FREQ_DIV;
    SSPSTAT = 0x00;
    SSPCON  = 0x28;         // I2C Master mode, clock = Fosc/(4*(SSPADD+1))
}

void I2C_Start() 
{
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN);
}

void I2C_Stop(void) {
    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN);
}

uint8_t I2C_WriteByte(uint8_t b)
{
    PIR1bits.SSPIF = 0;
    SSPBUF = b;
    while (!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
    return SSPCON2bits.ACKSTAT; // 0=ACK, 1=NACK
}

void main() 
{
    uint8_t val = 1;
    I2C_Init();
    __delay_ms(50);

    while (1) 
    {
        I2C_Start();
        I2C_WriteByte((0x50 << 1) | 0); // send slave addr (0x50) + write bit
        I2C_WriteByte(val++);
        I2C_Stop();
        if (val == 0) val = 1;
        __delay_ms(500);
    }
}
