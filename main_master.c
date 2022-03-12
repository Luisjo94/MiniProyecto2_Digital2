/*
 * Mini Proyecto 2: Master
 * Luis José Archila, 20403
 * Electrónica Digital 2
 */

/*----------------------------------------------------------------------------*/
/*                           BITS DE CONFIGURACIÓN                            */
/*----------------------------------------------------------------------------*/

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <pic16f887.h>

#include "I2C_CONF.h"
#include "LCD_conf.h"
#include "UART.h"

#define _XTAL_FREQ 4000000


/*----------------------------------------------------------------------------*/
/*                                  VARIABLES                                 */
/*----------------------------------------------------------------------------*/
uint8_t temp = 0;
char semaforo;
uint8_t a, b;

/*----------------------------------------------------------------------------*/
/*                                 PROTOTIPOS                                 */
/*----------------------------------------------------------------------------*/

void setup(void);
void MostarTemperatura(void);
char int_to_ascii(uint8_t);
void semaforo_lcd(uint8_t);

/*----------------------------------------------------------------------------*/
/*                               INTERRUPCIONES                               */
/*----------------------------------------------------------------------------*/

void __interrupt() isr (void)
{
    if (PIR1bits.RCIF)
    {
        semaforo = RCREG;
        b = semaforo - 48;
        PIR1bits.RCIF = 0;
    }
}



/*----------------------------------------------------------------------------*/
/*                                  MAIN LOOP                                 */
/*----------------------------------------------------------------------------*/

void main(void)
{
    setup();
    init_LCD();
    UartSetup();
    I2C_MasterMode(100000);
    
    SetCursor(0x0);
    WriteString("Temp:  ");
    WriteString("Semaforo:");
    
    //SetCursor(0x48);
    //WriteString("Amarillo");
    //WriteString("--------");
    
    semaforo = 2;
    
    while (1)
    {
        
        
        
        /* Mostrar semaforo en lcd */
        semaforo_lcd(semaforo);        
        
        
        /*
                0 = escribir
                1 = leer
         */
        
        
        /* ---- Leer valores del sensor ---- */
        I2C_Start();
        I2C_WriteAddress(0x91);
        temp = I2C_Read();
        I2C_Stop();
        //__delay_ms(5);
        
        /* ---- Enviar el valor del semaforo ---- */
        I2C_Start();
        I2C_WriteAddress(0x50);
        I2C_Write(semaforo);
        I2C_Stop();
        //__delay_ms(5);
        
        
        /* ---- Mostrar la temperatura en la LCD ---- */
        //MostrarTemperatura(temp);
        uint8_t tu, td;
        tu = temp%10;
        td = (temp/10)%10;
        
        SetCursor(0x41);
        WriteChar(int_to_ascii(td));
        WriteChar(int_to_ascii(tu));
        WriteChar(223);
        WriteChar('C');
        
        
        
        /* ---- UART con ESP32 ---- */
        if (PIR1bits.TXIF)
        {
            a = temp+48;
            UartChar(a);
        }
        

    }
}


/*----------------------------------------------------------------------------*/
/*                                  FUNCIONES                                 */
/*----------------------------------------------------------------------------*/

void setup(void)
{
    // port_conf
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    //TRISC = 0b000011000;    //SCL como input y SDA como input
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    //TRISCbits.TRISC6 = 0;   //TX
    //TRISCbits.TRISC7 = 1;   //RX
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    
    //osc_conf
    OSCCONbits.IRCF = 0b110; //4 MHz
    OSCCONbits.SCS = 1;      //internal used for system clk

    return;
}


void MostrarTemperatura(void)
{
    char tu, td;
    
    tu = temp%10;
    td = (temp/10)%10;
    
    SetCursor(0x41);
    WriteChar(td);
    WriteChar(tu);
    WriteChar(223);
    WriteChar('C');
    
    return;
}


char int_to_ascii(uint8_t a)
{
    return a+48;
}


void semaforo_lcd(uint8_t semaforo)
{
    switch(semaforo)
    {
        case 0:
            SetCursor(0x48);
            WriteString("Verde");
            break;
            
        case 1:
            SetCursor(0x48);
            WriteString("Amarillo");
            break;
            
        case 2:
            SetCursor(0x48);
            WriteString("Rojo");
            break;
            
        default:
            SetCursor(0x48);
            WriteString("--------");
            break;
    }
}