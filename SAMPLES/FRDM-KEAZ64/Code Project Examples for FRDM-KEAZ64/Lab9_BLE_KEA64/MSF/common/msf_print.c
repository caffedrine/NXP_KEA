/****************************************************************************
 * @file     msf_print.c
 * @brief    Print to serial line (console) - MSF global functions 
 * @version  2 - now using buffered i/o - coniob
 * @date     23. Apr 2014
 *
 * @note    Prototypes of these functions are in msf.h
 *      Uses msf conio driver to output the strings. 
 *
 ******************************************************************************/

#include "msf_config.h"                    

/* Include hardware definitions */
#include "coredef.h"   

#include "msf.h"
#include "coniob.h"  /* generic console driver with buffer */
#include <stdio.h> /* for sprintf */


/* print string */
void msf_print(const char* str)    // print string
{
    coniob_puts(str);
} 

/* print single character */
void msf_print_char(char c)	
{
	coniob_putch(c);
}

/* Print simple integer (as with sprintf %d) */
void msf_printnum(uint32_t number) 
{
    char buffer[12];
    sprintf(buffer, "%ld", number);
    coniob_puts(buffer);    
}

/*Print simple integer as hexadecimal number (as with sprintf %x) */
void msf_printhex(uint32_t number)
{
    char buffer[9];
    sprintf(buffer, "%lx", number);
    coniob_puts(buffer);         
}

/* print string with one formatted 16-bit number */
void msf_printf_16(const char* str, const char* format, uint16_t data)  
{
    char buffer[9];
    sprintf(buffer, format, data);
    coniob_puts(str);
    coniob_putch(' ');
    coniob_puts(buffer);    
}

/* print string with one formatted 32-bit number */ 
void msf_printf32(const char* str, const char* format, uint32_t data)  
{
    char buffer[12];
    sprintf(buffer, format, data);
    coniob_puts(str);
    coniob_putch(' ');
    coniob_puts(buffer);    
} 

/* print string with one real number (float) */ 
void msf_printf_real(const char* str, const char* format, double data)  
{
    char buffer[12];
    sprintf(buffer, format, data);
    coniob_puts(str);
    coniob_putch(' ');
    coniob_puts(buffer);    
} 


/* Read character from serial interface. */
char msf_read_char(void)
{
    return coniob_getch();    
}

/* Check if character was received through serial line */
bool msf_char_available(void)
{
    return (coniob_kbhit() > 0);
}


