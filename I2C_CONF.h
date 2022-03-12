#ifndef I2C_CONF
#define	I2C_CONF

#define lcd_port    PORTA 
#define RS  PORTCbits.RC0 
#define E   PORTCbits.RC1

#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdint.h>

void I2C_MasterMode(unsigned long clk);
void I2C_SlaveMode(uint8_t add);

unsigned short I2C_Read(void);
void I2C_Write(const uint8_t data);
void I2C_WriteAddress(uint8_t address);

void I2C_Start(void);
void I2C_Wait(void);
void I2C_Stop(void);

#endif	

