#include "vote_encoding.h"
#include "tlpi_hdr.h"

/**
 * the binary format uses fixed-size messages
 * Each message begins with a one-byte field that contains the “magic” 
 * value 010101 in its high-order 6 bits
 * 
 * Wire Format:
 * 
 *     0  1  2  3  4  5  6  7  8  9  1  2  3  4  5
 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 0 |   Magic         |Flags|     Zero           |
 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 1 |               Candidate id                 |
 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 2 |                                            |
 * 3 |         Vote count (only in response)      |
 * 4 |                                            |
 * 5 |                                            |
 *   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * */

enum {
    REQUEST_SIZE = 4,
    RESPONSE_SIZE = 12,
    COUNT_SHIFT = 32,
    INQUIRE_FLAG = 0x0100,
    RESPONSE_FLAG = 0x0200,
    MAGIC = 0x5400,
    MAGIC_MASK = 0xfc00
};

typedef struct {
    uint16_t header;
    uint16_t candidateID;
    uint32_t countHigh;
    uint32_t countLow;
} vote_msg_bin;



size_t encode(vote_info *v, uint8_t *buf, size_t size) {

}


bool decode(uint8_t *buf, size_t size, vote_info *v) {

}