#ifndef FAB_DICT
#define FAB_DICT

#include <stdbool.h>

/*
    - each key-val pair is stored in a node of a linked list
    - each linked list is pointed to by the element of an array
    - the array is indexed by the hash value of the keys
*/

typedef struct node {
    struct node *next;
    char *key; 
    char *val;
} node_t;

/**
 * Adds a new key-val pair or update the value of an existing key
 * 
 * Returns a pointer to the key-val pair created/modified or NULL if there is no 
 * enough memory for a new entry
 **/
node_t* add(char *key, char *val);

/**
 *  Returns a pointer to key-val pair or NULL if 'key' is not found
 **/
node_t* lookup(char *key);

/**
 *  Deletes a key-val pair
 **/
bool delete(char *key);

/**
 * Prints out the elements of the dictionary
 **/
void print();

#endif
