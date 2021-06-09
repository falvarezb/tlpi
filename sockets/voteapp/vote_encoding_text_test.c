#include "vote_encoding.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>


#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)

typedef struct state {
    size_t bufsize;
    uint8_t *buf;
} bufstate_t;

int setup(void** state){
    // bufstate_t *bufstate = malloc(sizeof(bufstate_t));
    // bufstate->bufsize = 50;
    // bufstate->buf = malloc(bufstate->bufsize * sizeof(uint8_t));
    // *state = bufstate;
    return 0;
}

int tear_down(void** state){
    // void* ptr = (bufstate_t*) ((*state)->bufstate);
    // free(ptr);
    return 0;
}

void encode_inq_req(void** state){
    vote_info v = { 0, 1, true, false};
    size_t bufsize = 50;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting I  1");
    free(buf);
}

void encode_inq_res(void** state){
    vote_info v = { 0, 1, true, true};
    size_t bufsize = 50;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting I R 1 0");
    free(buf);
}

void encode_vote_req(void** state){
    vote_info v = { 0, 1, false, false};
    size_t bufsize = 50;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting V  1");
    free(buf);
}

void encode_vote_res(void** state){
    vote_info v = { 0, 1, false, true};
    size_t bufsize = 50;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting V R 1 0");
    free(buf);
}

void encode_insufficient_buffer_space(void** state){
    vote_info v = { 0, 1, true, false};
    size_t bufsize = 5;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    size_t tot_written = encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voti");
    free(buf);
}

void decode_inq_req(void** state){
    const char* msg = "Voting I  1";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) malloc(sizeof(vote_info));

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_true(v->isInquiry);
    assert_false(v->isResponse);
    free(v);
}

void decode_inq_res(void** state){
    const char* msg = "Voting I R 1 0";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) malloc(sizeof(vote_info));

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_true(v->isInquiry);
    assert_true(v->isResponse);
    free(v);
}

void decode_vote_req(void** state){
    const char* msg = "Voting V  1";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) malloc(sizeof(vote_info));

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_false(v->isInquiry);
    assert_false(v->isResponse);
    free(v);
}

void decode_vote_res(void** state){
    const char* msg = "Voting V R 1 0";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) malloc(sizeof(vote_info));

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_false(v->isInquiry);
    assert_true(v->isResponse);
    free(v);
}

int main(int argc, char const *argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(encode_inq_req),
        cmocka_unit_test(encode_inq_res),
        cmocka_unit_test(encode_vote_req),
        cmocka_unit_test(encode_vote_res),
        cmocka_unit_test(decode_inq_req),
        cmocka_unit_test(decode_inq_res),
        cmocka_unit_test(decode_vote_req),
        cmocka_unit_test(decode_vote_res),
        cmocka_unit_test(encode_insufficient_buffer_space)
    };
    return cmocka_run_group_tests(tests, setup, tear_down);
}