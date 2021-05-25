
#include <stdio.h>
#include <stdint.h>
#include "framer.h"
#include "tlpi_hdr.h"

#define DELIMITER '\n'

/**
 * Read up to size bytes or until delimiter, copying into the given buffer
 * Return the number of bytes placed in buf (exluding delimiter)
 * 
 * If buffer fills without encountering delimiter, or EOF is found after some data 
 * but before delimiter, negative count is returned
 * 
 * If stream ends before first byte, -1 is returned.
 **/
size_t get_next_msg(FILE *in, uint8_t *buf, size_t size) {
    size_t count = 0;
    int nextc;

    while (count < size) {
        nextc = getc(in);
        if(nextc == EOF) {
            errMsg("error when getting char");
            return count > 0 ? -count : -1;
        } else if (nextc == DELIMITER) {
            return count;
        }
        buf[count++] = nextc;        
    }
    return -size;
}

/**
 *  Fail if the byte sequence to be framed already contains the delimiter
 *  (no bit-stuffing is made)
 **/
size_t put_msg(uint8_t buf[], size_t size, FILE *out) {
    
    // Check for delimiter in message
    for (size_t i = 0; i < size; i++) {
        if(buf[i] == DELIMITER) {
            errMsg("delimiter found in message");
            return -1;
        }
    }

    if(fwrite(buf, 1, size, out) != size) {
        errMsg("error when writing message");
        return -1;
    }
    
    if(fputc(DELIMITER, out) == EOF) {
        errMsg("error when writing delimiter");
        return -1;
    }

    if(fflush(out) == EOF) {
        errMsg("error when flushing message");
        return -1;
    }
    
    return size;
}