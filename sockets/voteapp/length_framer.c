#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include "framer.h"
#include "tlpi_hdr.h"

/* Length-based framing */

/**
 * Read 2-byte length and place it in host byte order
 * Then read the indicated number of bytes
 * 
 * If buffer is not large enough to read the message, negative
 * buffer size is returned
 * 
 * If less bytes than expected are read (malformed message), negative count is returned
 * 
 * If message size cannot be read, -1 is returned
 * 
 * 
 **/
size_t get_next_msg(FILE *in, uint8_t *buf, size_t size) {

    uint16_t message_size = 0;

    if (fread(&message_size, sizeof(uint16_t), 1, in) != 1) {
        errMsg("error when reading mesage length"); 
        return -1;
    }

    message_size = ntohs(message_size);

    if(message_size > size)
        return -size;

    size_t num_read;
    if ((num_read = fread(buf, sizeof(uint8_t), message_size, in)) != message_size) {
        errMsg("Framing error: expected %d bytes, but found %d\n", message_size, num_read);        
        return -num_read;
    }

    return message_size;

}

/**
 * If message is longer than the maximum possible, negative message length is returned
 * 
 * Convert message length to network byte order and write 2-byte value. If write fails,
 * return -1
 * 
 * Then write buffer content. If write fails, return number of written bytes
 **/
size_t put_msg(uint8_t buf[], size_t size, FILE *out) {

    if(size > UINT16_MAX) {
        errMsg("message too long");
        return -size; 
    }

    uint16_t message_size = htons(size);
    
    if (fwrite(&message_size, sizeof(uint16_t), 1, out) != 1) {
       errMsg("error while writing message length"); 
       return -1;
    }

    size_t num_written;
    if ((num_written = fwrite(buf, sizeof(uint8_t), size, out)) != size) {
       errMsg("partial write: written %d bytes out of %d", num_written, size); 
       return -num_written;
    }

    if(fflush(out) == EOF) {
        errMsg("error when flushing message");
        return -1;
    }
    
    return num_written;

}