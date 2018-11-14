#include "CircularBuffer.h"

int circ_buff_push(circ_buff_t *c, uint16_t data)
{
	/* Update head */
	int next;
	if( (c->head) >= c->maxlen )
	{
		next = 0;
	}
	else
	{
		next = (c->head + 1);
	}
	c->buffer[c->head] = data;  // Load data and then move
	c->head = next;             // head to next data offset.
	
	/* Update tail */
	if(c->head == c->tail)
	{
		if(c->tail+1 >= c->maxlen)
			c->tail = 0;
		else
			c->tail += 1;
	}
	return 0;  // return success to indicate successful push.
}

int circ_buff_pop(circ_buff_t *c, uint16_t *data)
{
	/* Empty */
	if( c->head == c->tail )
		return -1;
	
	int next;
	if( (c->tail + 1) >= c->maxlen )
		next = 0;
	else
		next = c->tail + 1;

	*data = c->buffer[c->tail];  // Read data and then move
	c->tail = next;              // tail to next offset.
	return 0;  // return success to indicate successful push.
}

int circ_buff_free_space(circ_buff_t *c)
{
	int freeSpace;
	freeSpace = c->tail - c->head;
	if( freeSpace <= 0 )
		freeSpace += c->maxlen;
	return freeSpace; // -1 to account for the always-empty slot.
}

uint16_t circ_buff_average(circ_buff_t *c)
{
	if( c->head == c->tail )  // if the head == tail, we don't have any data
		return 0;

	uint32_t sum = 0;
	int elements = 0;
	
	int index = c->tail;
	while(1)
	{
		if(index == c->head)
			break;
		
		sum += c->buffer[index];
		elements++;
		
		if(index >= c->maxlen)
			index = 0;
		else
			index++;
	}

	return (uint16_t)(sum/elements);
}

int circ_buffer_elements(circ_buff_t *c)
{
	/* Empty */
	if( c->head == c->tail )
		return 0;
	
	uint16_t result = 0;
	if(c->head < c->tail)
	{
		result = (c->maxlen)-(c->tail); /* elements from right */
		result += (c->head+1);	/* elements form left */
	}
	else
	{
		return (c->head - c->tail);
	}
}