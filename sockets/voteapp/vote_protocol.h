#ifndef VOTE_PROTOCOL
#define VOTE_PROTOCOL

#include <stdint.h>

struct vote_info {
    uint64_t count;
    int candidate;
    bool isInquiry;
    bool isResponse;
};
// invariant: !isResponse => count==0
// invariant: 0 <= candidate <= MAX_CANDIDATE
typedef struct vote_info vote_info;
enum { 
    MAX_CANDIDATE = 1000, 
    MAX_WIRE_SIZE = 500 
};

#endif /* VOTE_PROTOCOL */