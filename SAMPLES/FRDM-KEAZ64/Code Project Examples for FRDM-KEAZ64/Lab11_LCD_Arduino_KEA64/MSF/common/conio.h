/****************************************************************************
 * @file     conio.h
 * @brief    Console I/O driver; generic definitions for all platforms 
 * @version  1
 * @date     22. Apr2014
 *
 * @note        
 *
 ******************************************************************************/
#ifndef MSF_CONIO_H
#define MSF_CONIO_H

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup group_conio  
 * @{
*/    

/* initialize console I/O */
void conio_init(void);  
/* send one character to console */
void conio_putch(char c);        
/* send null-terminated string */
void conio_puts(const char* str);     
/* read one character (blocking) */ 
char conio_getch(void);               
/* check if data are available in input buffer */ 
uint8_t conio_kbhit(void);             
/* read string from console. */
void conio_gets(char* str, uint8_t max_chars, char terminator);		 


/** @} */
#ifdef __cplusplus
}
#endif
/* ----------- end of file -------------- */
#endif /* MSF_CONIO_H */