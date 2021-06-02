#include "vote_encoding.h"
#include "tlpi_hdr.h"

#define enter() printf("enter: %s\n", __func__)

/**
 * Wire format:
 * "Voting <v|i> [R] <candidate ID> <count>" 
**/


static const char *MAGIC = "Voting";
static const char *VOTESTR = "v";
static const char *INQSTR = "i";
static const char *RESPONSESTR = "R";
static const char *DELIMSTR = " ";

enum {
    BASE = 10
};

/**
 * Encode voting message info as a text string.
 **/
size_t encode(vote_info *v, uint8_t *buf, size_t size) {
    enter();
    size_t num_written, tot_written;

    if((num_written = sprintf((char *) buf, size, "%s %c %s %d", MAGIC,(v->isInquiry ? 'i' : 'v'), (v->isResponse ? "R" : ""), v->candidate)) < 0) {
        errMsg("error while encoding message");
        return -1;
    }

    buf += num_written;
    size -= num_written;
    tot_written = num_written;

    if (v->isResponse) {
        if((num_written = sprintf((char *) buf, size, " %llu", v->count)) < 0){
            errMsg("error while encoding message");
            return -1;
        }
        tot_written += num_written;
    }

    return num_written;
}

/**
 * Extract message information from given buffer.
 * Note: modifies input buffer.
 * */
bool decode(uint8_t *buf, size_t size, vote_info *v) {
    
    char *token;

    //magic string
    token = strtok((char *) buf, DELIMSTR);
    if (token == NULL || strcmp(token, MAGIC) != 0)
        return false;

    //vote/inquiry
    token = strtok(NULL, DELIMSTR);
    if(token == NULL)
        return false;

    if (strcmp(token, VOTESTR) == 0)
        v->isInquiry = true;
    else if(trcmp(token, INQSTR) == 0)
        v->isInquiry = false;
    else
        return false;

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