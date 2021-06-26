#include "vote_encoding.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>


#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)

struct statebuf {
    size_t bufsize;
    uint8_t *buf;
};

int encode_setup(void** state){
    struct statebuf *statebuf = malloc(sizeof(struct statebuf));
    statebuf->bufsize = 50;
    statebuf->buf = malloc(statebuf->bufsize * sizeof(uint8_t));
    *state = statebuf;
    return 0;
}

int encode_tear_down(void** state){
    free((void*) (((struct statebuf*)(*state))->buf));
    free((void*) *state);
    return 0;
}

int decode_setup(void** state){
    *state = malloc(sizeof(vote_info));
    return 0;
}

int decode_tear_down(void** state){
    free((void*) *state);
    return 0;
}

void encode_inq_req(void** state){
    vote_info v = { 0, 1, true, false};
    size_t bufsize = ((struct statebuf*)(*state))->bufsize;    
    uint8_t *buf = ((struct statebuf*)(*state))->buf;

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting I  1");    
}

void encode_inq_res(void** state){
    vote_info v = { 0, 1, true, true};
    size_t bufsize = ((struct statebuf*)(*state))->bufsize;    
    uint8_t *buf = ((struct statebuf*)(*state))->buf;

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting I R 1 0");
}

void encode_vote_req(void** state){
    vote_info v = { 0, 1, false, false};
    size_t bufsize = ((struct statebuf*)(*state))->bufsize;    
    uint8_t *buf = ((struct statebuf*)(*state))->buf;

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting V  1");
}

void encode_vote_res(void** state){
    vote_info v = { 0, 1, false, true};
    size_t bufsize = ((struct statebuf*)(*state))->bufsize;    
    uint8_t *buf = ((struct statebuf*)(*state))->buf;

    encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting V R 1 0");
}

void encode_insufficient_buffer_space(void** state){
    (void) state;
    vote_info v = { 0, 1, false, true};
    size_t bufsize = 10;
    uint8_t *buf = malloc(bufsize * sizeof(uint8_t));

    size_t tot_written = encode(&v, buf, bufsize);    
    assert_string_equal(buf, "Voting V ");
    assert_int_equal(tot_written, strlen("Voting V R 1"));
    free(buf);
}

void decode_inq_req(void** state){
    const char* msg = "Voting I  1";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) *state;

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_true(v->isInquiry);
    assert_false(v->isResponse);
}

void decode_inq_res(void** state){
    const char* msg = "Voting I R 1 0";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) *state;

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_true(v->isInquiry);
    assert_true(v->isResponse);
}

void decode_vote_req(void** state){
    const char* msg = "Voting V  1";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) *state;

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_false(v->isInquiry);
    assert_false(v->isResponse);
}

void decode_vote_res(void** state){
    const char* msg = "Voting V R 1 0";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) *state;

    bool result = decode((uint8_t*) buf, size, v);
    assert_true(result);
    assert_int_equal(v->count, 0);
    assert_int_equal(v->candidate, 1);
    assert_false(v->isInquiry);
    assert_true(v->isResponse);
}

void decode_incorrect_msg(void** state){
    const char* msg = "fljljjl";
    char* buf = strndup(msg, strlen(msg));
    size_t size = strlen(buf);
    vote_info *v = (vote_info*) *state;

    bool result = decode((uint8_t*) buf, size, v);
    assert_false(result);
}

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(encode_inq_req, encode_setup, encode_tear_down),
        cmocka_unit_test_setup_teardown(encode_inq_res, encode_setup, encode_tear_down),
        cmocka_unit_test_setup_teardown(encode_vote_req, encode_setup, encode_tear_down),
        cmocka_unit_test_setup_teardown(encode_vote_res, encode_setup, encode_tear_down),
        cmocka_unit_test_setup_teardown(encode_insufficient_buffer_space, encode_setup, encode_tear_down),
        cmocka_unit_test_setup_teardown(decode_inq_req, decode_setup, decode_tear_down),
        cmocka_unit_test_setup_teardown(decode_inq_res, decode_setup, decode_tear_down),
        cmocka_unit_test_setup_teardown(decode_vote_req, decode_setup, decode_tear_down),
        cmocka_unit_test_setup_teardown(decode_vote_res, decode_setup, decode_tear_down),        
        cmocka_unit_test_setup_teardown(decode_incorrect_msg, decode_setup, decode_tear_down)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}