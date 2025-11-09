// CONFIG
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL
#include <xc.h>

void UART_Init(void) 
{
    SPBRG = 25;
    TXSTAbits.BRGH = 1;   // high speed
    TXSTAbits.SYNC = 0;   // asynchronous
    TXSTAbits.TXEN = 1;   // enable TX  
    RCSTAbits.SPEN = 1;   // enable serial port
}

void UART_Write(unsigned char data) 
{
    while(!PIR1bits.TXIF);  // wait for buffer empty
    TXREG = data;
}

void UART_Write_Text(const char *text) 
{
    while(*text) 
    {
        UART_Write(*text++);
    }
}

void main() 
{
    OSCCON = 0x61;  // 4 MHz
    ANSEL = 0; 
    ANSELH = 0;
    
    UART_Init();
    
    while(1) 
    {
        UART_Write_Text("Hello World\r\n");
        __delay_ms(1000);
    }
}