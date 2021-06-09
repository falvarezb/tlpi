#include "vote_encoding.h"
#include "tlpi_hdr.h"

#define enter() printf("enter: %s\n", __func__)

/**
 * Wire format (only ASCII chars):
 * "Voting <v|i> [R] <candidate ID> <count>" 
**/


static const char *MAGIC = "Voting";
static const char *INQSTR = "I";
static const char *VOTESTR = "V";
static const char *RESPONSESTR = "R";
static const char *DELIMSTR = " ";

enum {
    BASE = 10
};

/**
 * Encode voting message info as a text string.
 * 
 * Example:
 * inquiry req: "Voting i  12"
 * inquiry res: "Voting i R 12 20"
 * vote req: "Voting v  12"
 * vote res: "Voting v R 12 20"
 * 
 **/
size_t encode(vote_info *v, uint8_t *buf, size_t size) {
    //enter();
    size_t num_written, tot_written;

    if((num_written = snprintf((char *) buf, size, "%s %s %s %d", MAGIC,(v->isInquiry ? INQSTR : VOTESTR), (v->isResponse ? RESPONSESTR : ""), v->candidate)) < 0) {
        errMsg("error while encoding message");
        return -1;
    }

    buf += num_written;    
    size -= num_written;
    tot_written = num_written;

    if (v->isResponse) {
        if((num_written = snprintf((char *) buf, size, " %llu", v->count)) < 0){
            errMsg("error while encoding message");
            return -1;
        }
        tot_written += num_written;
    }

    return tot_written;
}

/**
 * Extract message information from given buffer.
 * Note: modifies input buffer.
 * */
bool decode(uint8_t *buf, size_t size, vote_info *v) {

    //cast to char* is justified because the content of buf is ASCII (0-127),
    //therefore no change of sign will happen when casting    
    char *token = strtok((char *) buf, DELIMSTR);
    
    //magic string
    if (token == NULL || strcmp(token, MAGIC) != 0)
        return false;

    //vote/inquiry
    token = strtok(NULL, DELIMSTR);
    if(token == NULL)
        return false;

    if (strcmp(token, INQSTR) == 0)
        v->isInquiry = true;
    else
        v->isInquiry = false;

    // Next token is either Response flag or candidate ID
    token = strtok(NULL, DELIMSTR);
    if(token == NULL)
        return false;

    if (strcmp(token, RESPONSESTR) == 0) {
        v->isResponse = true;
        token = strtok(NULL, DELIMSTR); // Get candidate ID
        if (token == NULL)
            return false;
    }
    else
        v->isResponse = false;

    v->candidate = atoi(token);
    if(v->isResponse){ // Response message should contain a count value
        token = strtok(NULL, DELIMSTR);
        if (token == NULL)
            return false;
        v->count = strtoll(token, NULL, BASE);
    } else {
        v->count = 0L;
    }
    
    return true;
}