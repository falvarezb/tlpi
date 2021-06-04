#include "dict.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

void add_new_entry(void** state){
    (void) state; /* unused */

    node_t* entry = add("mykey", "myval");
    assert_non_null(entry);
    assert_string_equal(entry->key, "mykey");
    assert_string_equal(entry->val, "myval");
    print();
}

void lookup_entry(void** state){
    (void) state; /* unused */

    node_t* entry = lookup("mykey");
    assert_non_null(entry);
    assert_string_equal(entry->key, "mykey");
    assert_string_equal(entry->val, "myval");
}

void lookup_nonexistent_entry(void** state){
    (void) state; /* unused */

    node_t* entry = lookup("randomkey");
    assert_null(entry);
}

void modify_existing_entry(void** state){
    (void) state; /* unused */

    node_t* entry = add("mykey", "newval");      
    assert_string_equal(entry->key, "mykey");
    assert_string_equal(entry->val, "newval");
    print();
}

void remove_only_existing_entry(void** state){
    (void) state; /* unused */
    
    delete("mykey");   
    node_t* removed_entry = lookup("mykey");
    assert_null(removed_entry);  
    print();    
}

void remove_first_entry(void** state){
    (void) state; /* unused */
    
    //'fc' and 'key' have the same hash value
    add("fc", "val2");
    add("key", "val1");
    print();
    delete("key");   
    node_t* entry = lookup("key");
    assert_null(entry);  
    entry = lookup("fc");
    assert_non_null(entry);  
    print(); 
}

void remove_last_entry(void** state){
    (void) state; /* unused */
    
    add("key", "val1");
    print();
    delete("fc");   
    node_t* entry = lookup("fc");
    assert_null(entry);  
    entry = lookup("key");
    assert_non_null(entry);  
    print(); 
}

void printer(void** state){
    (void) state; /* unused */
    
    add("fc", "val2");
    add("key", "val1");
    add("flc", "val3");
    add("kuey", "val4");
    print(); 
}

int main(int argc, char const *argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(add_new_entry),
        cmocka_unit_test(lookup_entry),
        cmocka_unit_test(lookup_nonexistent_entry),
        cmocka_unit_test(modify_existing_entry),
        cmocka_unit_test(remove_only_existing_entry),
        cmocka_unit_test(remove_first_entry),
        cmocka_unit_test(remove_last_entry),
        cmocka_unit_test(printer)         
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}