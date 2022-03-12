#ifndef UART_H
#define	UART_H

#include <xc.h> 
#include <string.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000

void UartSetup(void);
void UartChar(char a);
void UartString(char txt[]);

#endif

