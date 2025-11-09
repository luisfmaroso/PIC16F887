#include "display.h"

static short d0, d1, d2, d3;
static short pd0, pd1, pd2, pd3;

void lcd_pulse()
{
    E = 1;
    __delay_ms(5);
    E = 0;
}

void lcd_init()
{
    E = 0;
    RS = 0;
    DATA_PORT = 0x38; 
    lcd_pulse();
    DATA_PORT = 0x38; 
    lcd_pulse();
    DATA_PORT = 0x06; 
    lcd_pulse();
    DATA_PORT = 0x0C; 
    lcd_pulse();
    DATA_PORT = 0x01; 
    lcd_pulse();
    RS = 1;
}

void lcd_write(unsigned char position, char t[])
{
    unsigned char i = 0;
    RS = 0;
    DATA_PORT = position; 
    lcd_pulse();
    RS = 1;
    while (t[i] != 0)
    {
        DATA_PORT = (unsigned char)t[i]; 
        lcd_pulse();
        i++;
    }
}

void lcd_clear(void)
{
    RS = 0;
    DATA_PORT = 0x01;
    lcd_pulse();
    RS = 1;
}

void convert_period(void)
{
    d3 = (short)(period_us / 10000u);
    d2 = (short)((period_us % 10000u) / 1000u);
    d1 = (short)((period_us % 1000u) / 100u);
    d0 = (short)((period_us % 100u) / 10u);
}

void update_display(void)
{
    if (d3 != pd3) 
    {
        RS = 0; DATA_PORT = 0xC7;
        lcd_pulse();
        RS = 1; pd3 = d3; DATA_PORT = (unsigned char)(d3 + '0'); lcd_pulse();
    }
    if (d2 != pd2) 
    {
        RS = 0; DATA_PORT = 0xC8;
        lcd_pulse();
        RS = 1; pd2 = d2; DATA_PORT = (unsigned char)(d2 + '0'); lcd_pulse();
    }
    if (d1 != pd1) 
    {
        RS = 0; DATA_PORT = 0xCA;
        lcd_pulse();
        RS = 1; pd1 = d1; DATA_PORT = (unsigned char)(d1 + '0'); lcd_pulse();
    }
    if (d0 != pd0) 
    {
        RS = 0; DATA_PORT = 0xCB;
        lcd_pulse();
        RS = 1; pd0 = d0; DATA_PORT = (unsigned char)(d0 + '0'); lcd_pulse();
    }
}