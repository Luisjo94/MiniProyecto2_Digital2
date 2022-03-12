#include "UART.h"


void UartSetup(void)
{
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 8;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    TXSTAbits.TX9 = 0; 
}

void UartChar(char a)
{
    while(!TXSTAbits.TRMT);
    TXREG = a;
    __delay_ms(3000);
}

void UartString(char txt[])
{
    uint8_t i;
    for (i=0; i <= strlen(txt); i++)
    {
        UartChar(txt[i]);
    }
}
