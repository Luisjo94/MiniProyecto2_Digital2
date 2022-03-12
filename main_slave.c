/*
 * Mini Proyecto 2: Slave
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
#pragma config WDTE = ON       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
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
#include <pic16f887.h>

#include "I2C_CONF.h"

#define _XTAL_FREQ 4000000


/*----------------------------------------------------------------------------*/
/*                                  VARIABLES                                 */
/*----------------------------------------------------------------------------*/
uint8_t semaforo;
uint8_t z;

/*----------------------------------------------------------------------------*/
/*                                 PROTOTIPOS                                 */
/*----------------------------------------------------------------------------*/

void setup(void);
void estado(uint8_t);

/*----------------------------------------------------------------------------*/
/*                               INTERRUPCIONES                               */
/*----------------------------------------------------------------------------*/

void __interrupt() isr (void)
{
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            __delay_us(7);
            //while (!SSPSTATbits.BF);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            __delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            //while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            //semaforo = SSPBUF;          // Guardar en semaforo el valor del buffer de recepción
            //__delay_us(250);
            
        }
        
        if (SSPSTATbits.D_nA && !SSPSTATbits.R_nW)
        {
            
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            semaforo = SSPBUF;          // Guardar en semaforo el valor del buffer de recepción
            PIR1bits.SSPIF = 0;
            SSPCONbits.CKP = 1;
            //__delay_us(250);
        }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = 0;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}



/*----------------------------------------------------------------------------*/
/*                                  MAIN LOOP                                 */
/*----------------------------------------------------------------------------*/

void main(void)
{
    setup();
    I2C_SlaveMode(0x50);
    
    //semaforo = 2;
    
    while (1)
    {
        estado(semaforo);
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
    TRISC = 0; 
    //TRISCbits.TRISC3 = 1;
    //TRISCbits.TRISC4 = 1;
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
    
    //interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;

    return;
}


void estado(uint8_t s)
{
    switch(s)
    {
        case 0:         //GREEN
            PORTE = 0b001;
            break;
        case 1:         //YELLOW
            PORTE = 0b010;
            break;
        case 2:         //RED
            PORTE = 0b100;
            break;
        default:
            PORTE = 0;
            break;
    }
}