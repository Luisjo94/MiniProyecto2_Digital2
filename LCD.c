#include "LCD_conf.h"
#define _XTAL_FREQ 4000000

#define lcd_port    PORTA 
#define RS  PORTCbits.RC0 
#define E   PORTCbits.RC1

void pulse(void)
{
    E = 1;
    __delay_ms(4);
    E = 0;
    return;
}

void init_LCD(void)
{
    //inicializacion
    __delay_ms(20);
    RS = 0;  //RS
    lcd_port = 0b00110000;
    pulse();
    
    __delay_ms(10);
    RS = 0;
    lcd_port = 0b00110000;
    pulse();
    
    __delay_us(150);
    RS = 0;
    lcd_port = 0b00110000;
    pulse();
    
    
    //activar visualizador
    RS = 0;
    lcd_port = 0b00111000; //38
    pulse();
    
    //activar cursor
    RS = 0;
    lcd_port = 0b00001100; //0C
    pulse();
    
    //clear
    RS = 0;
    lcd_port = 0x01;
    pulse();

    RS = 0;
    lcd_port = 0x06;
    pulse();
    
    return;
}


void WriteChar(char a)
{
    RS = 1;  //RS
    lcd_port = a;
    pulse();
}


void WriteString(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
    {
	   WriteChar(a[i]);
    }
}


void SetCursor(uint8_t p)
{
    RS = 0;
    lcd_port = 0b10000000 + p;
    pulse();
    __delay_ms(4);
}