#include <xc.h>
#include <string.h>

#define _XTAL_FREQ 4000000

#define RS   PORTEbits.RE0
#define E    PORTEbits.RE1
#define DATA_PORT PORTD

void lcd_pulse(void);
void lcd_init(void);
void lcd_write(unsigned char position, char t[]);
void lcd_clear(void);
void convert_period(void);
void update_display(void);

extern volatile unsigned int period_us;
extern char text[17];