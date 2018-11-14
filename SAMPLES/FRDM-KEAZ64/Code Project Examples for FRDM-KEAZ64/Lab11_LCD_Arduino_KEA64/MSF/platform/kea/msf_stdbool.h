/****************************************************************************
 * @file     stdbool.h
 * @brief    define C99 data type bool
 * @version  1
 * @date     28. Jan 2014
 *
 * @note    Defines some C99 data types for compilers which does not contain this file.
 *
 ******************************************************************************/
#ifndef MSF_STDBOOL_H
	#define MSF_STDBOOL_H

#ifdef __cplusplus
extern "C" {
#endif
/* Our own bool definition
 For C99 compiler it is contained in stdbool.h*/
//typedef enum { false, true } bool;
#ifndef bool
enum ucp_bool_type { false, true };
typedef enum ucp_bool_type bool;
#endif

/*typedef uint8_t bool;
#define	true	1
#define	false	0
*/    
	
#ifdef __cplusplus
}
#endif

#endif /* MSF_STDBOOL_H */
