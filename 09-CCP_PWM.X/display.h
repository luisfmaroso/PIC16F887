#ifndef DISPLAY_H
#define DISPLAY_H

#include <xc.h>

#define _XTAL_FREQ 4000000UL

#define RS PORTEbits.RE0
#define E PORTEbits.RE1
#define DATA_PORT PORTD

void lcd_init(void);
void lcd_write(unsigned char position, char t[]);
void lcd_clear(void);
void lcd_pulse(void);

#define display_init lcd_init
#define display_clear lcd_clear

#endif