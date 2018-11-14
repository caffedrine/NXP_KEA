#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <stdint.h>

typedef struct
{
	uint16_t * const buffer;
	int head;
	int tail;
	const int maxlen;
} circ_buff_t;

/*
 * Example:
 * CIRC_buff_DEF(circ_buf_name, 32);
 */

#define CIRC_buff_DEF(x,y)                	\
    uint16_t x##_data_space[y];          	\
    circ_buff_t x = 						\
	{                     					\
        .buffer = x##_data_space,         	\
        .head = 0,                        	\
        .tail = 0,                        	\
        .maxlen = y                     	\
    }

/*
 * Method: circ_buf_pop
 * Returns:
 *  0 - Success
 * -1 - Empty
 */
int circ_buff_pop(circ_buff_t *c, uint16_t *data);

/*
 * Method: circ_buf_push
 * Returns:
 *  0 - Success
 * -1 - Out of space
 */
int circ_buff_push(circ_buff_t *c, uint16_t data);

/*
 * Method: circ_buff_free_space
 * Returns: number of bytes available
 */
int circ_buff_free_space(circ_buff_t *c);

/*Method: circ_buff_average
 *  0 - Success
 * -1 - Error/no elements
 */
uint16_t circ_buff_average(circ_buff_t *c);

int circ_buffer_elements(circ_buff_t *c);

#endif /* _CIRCULARBUFFER_H_ */
