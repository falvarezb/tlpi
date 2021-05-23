#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


// Warning:  Untested preconditions (e.g., 0 <= size <= 8)
int encode_big_endian(uint8_t dst[], uint64_t val, int offset, int size) {    
    for (int i = 0; i < size; i++) {
        dst[offset++] = (uint8_t) (val >> (((size - 1) - i) * CHAR_BIT));
    }
    return offset;
}

int encode_little_endian(uint8_t dst[], uint64_t val, int offset, int size) {    
    for (int i = size-1; i >= 0; i--) {
        dst[offset++] = (uint8_t) (val >> (((size - 1) - i) * CHAR_BIT));
    }
    return offset;
}

uint64_t decode_big_endian(uint8_t val[], int offset, int size) {
    uint64_t rtn = 0;
    for (int i = 0; i < size; i++) {
        rtn = (rtn << CHAR_BIT) | val[offset + i];
    }
    return rtn;
}

int main(int argc, char const *argv[])
{
    //When a signed value is copied to any wider type, the additional bits are copied from the sign (i.e., most significant) bit
    //when expressions are evaluated, values of variables are widened (if needed) to the “native” (int) size before any computation occurs
    // int8_t a = -30;
    // uint16_t b = a;
    // //a = b;
    // printf("b=%d", b);

    // char x,y;
    // printf("sizeof(x+y) is %lu\n", sizeof(x+y));
    // printf("CHAR_BIT is %d\n\n",CHAR_BIT);

    uint8_t dst[24];
    memset(dst, 0, 24);

    int offset = encode_little_endian(dst, 5485989889, 0, 8);
    printf("offset=%d\n", offset);
    printf("dst=%s\n", dst);

    return 0;
}



