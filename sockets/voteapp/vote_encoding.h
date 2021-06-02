#ifndef VOTE_ENCODING
#define VOTE_ENCODING

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "vote_protocol.h"

/**
 * takes a byte sequence of a specified size and parses it as a message according to the protocol, 
 * filling in the vote_info with the information from the message. 
 * 
 * It returns true if the message was successfully parsed, and false otherwise
 **/
bool decode(uint8_t *buf, size_t size, vote_info *v);

/**
 * takes a vote_info structure as input and converts it to a sequence of bytes according 
 * to a particular wire format encoding; it returns the size of the resulting byte sequence
 * 
 * it returns the size of the resulting byte sequence
 **/
size_t encode(vote_info *v, uint8_t *buf, size_t size);


#endif /* VOTE_ENCODING */