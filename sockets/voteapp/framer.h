#ifndef FRAMER
#define FRAMER

#include <stdio.h>
#include <stdint.h>

/**
 * reads data from the given stream and places it in the given buffer until it runs out of 
 * room or determines that it has received  a complete message
 * 
 * It returns the number of bytes placed in the buffer (all framing information is stripped).
 **/
size_t get_next_msg(FILE *in, uint8_t *buf, size_t size);

/**
 * adds framing information to the message contained in the given buffer, and writes 
 * both message and framing information to the given stream
 * 
 * Return number of bytes written, or -1 on failure.
 **/
size_t put_msg(uint8_t buf[], size_t size, FILE *out);

#endif /* FRAMER */