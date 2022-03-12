#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 
#include <stdint.h>

void init_LCD(void);
void WriteChar(char a);
void SetCursor(uint8_t p);
void WriteString(char *a);

#endif

