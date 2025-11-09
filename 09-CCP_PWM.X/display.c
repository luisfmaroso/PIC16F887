#include "display.h"

void lcd_pulse()
{
    E = 1;
    __delay_ms(5);
    E = 0;
}

void lcd_init()
{
    TRISEbits.TRISE0 = 0;  // RE0 (RS) as output
    TRISEbits.TRISE1 = 0;  // RE1 (E) as output
    TRISD = 0x00;  // PORTD as output (data)
    
    E = 0;
    RS = 0;
    __delay_ms(20);
    
    DATA_PORT = 0x38; lcd_pulse();  // 8-bit mode, 2 lines
    __delay_ms(5);
    DATA_PORT = 0x38; lcd_pulse();
    __delay_ms(1);
    DATA_PORT = 0x38; lcd_pulse();
    DATA_PORT = 0x0C; lcd_pulse();  // display ON, cursor OFF
    DATA_PORT = 0x06; lcd_pulse();  // entry mode
    DATA_PORT = 0x01; lcd_pulse();  // clear display
    __delay_ms(2);
    
    RS = 1;
}

void lcd_write(unsigned char position, char t[])
{
    unsigned char i = 0;
    RS = 0;
    DATA_PORT = position; lcd_pulse();
    RS = 1;
    while (t[i] != 0)
    {
        DATA_PORT = (unsigned char)t[i]; lcd_pulse();
        i++;
    }
}

void lcd_clear()
{
    RS = 0;
    DATA_PORT = 0x01; 
    lcd_pulse();
    __delay_ms(2);
    RS = 1;
}