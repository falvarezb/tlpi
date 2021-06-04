#include "dict.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
//#include <stdio.h>

// char* strdup(char* s) {
//     printf("hello");    
//     return NULL;
// }

void add_new_entry(void** state){
    (void) state; /* unused */

    node_t* new_entry = add("mykey", "myval");
    assert_non_null(new_entry);
    assert_string_equal(new_entry->key, "mykey");
    assert_string_equal(new_entry->val, "myval");
}

void lookup_entry(void** state){
    (void) state; /* unused */

    node_t* found_entry = lookup("mykey");
    assert_non_null(found_entry);
    assert_string_equal(found_entry->key, "mykey");
    assert_string_equal(found_entry->val, "myval");
}

void modify_existing_entry(void** state){
    (void) state; /* unused */

    node_t* found_entry = lookup("mykey");
    assert_non_null(found_entry);
    assert_string_equal(found_entry->val, "myval");
    
    node_t* modified_entry = add("mykey", "newval");      
    assert_string_equal(modified_entry->key, "mykey");
    assert_string_equal(modified_entry->val, "newval");
}

int main(int argc, char const *argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(add_new_entry),
        cmocka_unit_test(lookup_entry),
        cmocka_unit_test(modify_existing_entry)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}