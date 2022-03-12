#include "I2C_CONF.h"


void I2C_MasterMode(unsigned long clk)
{
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    SSPSTATbits.SMP = 1; //slew rate control disabled - standard speed mode
    //SSPSTATbits.CKE = 0; //data trans. on rising edge
    
    //SSPSTAT = 0;
    
    SSPCON2 = 0;
    
    //SSPCONbits.SSPEN =  1;      //pins used in I2C
    //SSPCONbits.SSPM = 0b1000;   //Master mode
    
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    
    
    
    SSPADD = (_XTAL_FREQ/(4*clk))-1;

    //SSPCON = 0b00101000;
    //SSPCON2 = 0;
    //SSPADD = (_XTAL_FREQ/(4*clk))-1;
    //SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_SlaveMode(uint8_t add)
{
    /*
    SSPSTATbits.SMP = 1; //slew rate control disabled - standard speed mode **revisar**
    SSPSTATbits.CKE = 0; //data trans. on rising edge
    
    SSPCONbits.SSPEN = 1;   //pins used in i2c
    SSPCONbits.SSPM = 0b0110;   //7bit address slave mode
    
    SSPCON2bits.SEN = 1;
    */
    
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    
    SSPADD = add;
}


unsigned short I2C_Read(void)
{
    unsigned short x;
    
    I2C_Wait();
    SSPCON2bits.RCEN = 1; //habilitar la recepcion
    I2C_Wait();
    x = SSPBUF;
    I2C_Wait();
    SSPCON2bits.ACKDT = 1;  //enviar bit de reconocimiento
    SSPCON2bits.ACKEN = 1;  //iniciar secuencia de reconocimiento
    return x;
}

void I2C_WriteAddress(uint8_t address)
{
    I2C_Wait();
    SSPBUF = address;
}

void I2C_Start(void)
{
    I2C_Wait();
    SSPCON2bits.SEN = 1;    //empezar comunicacion
}

void I2C_Write(const uint8_t data)
{
    I2C_Wait();
    SSPBUF = data;
}

void I2C_Stop(void)
{
    I2C_Wait();
    SSPCON2bits.PEN = 1;            //secuencia de stop
}

void I2C_Wait(void)
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
    //__delay_ms(45);
}