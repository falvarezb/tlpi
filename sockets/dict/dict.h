#ifndef DICT
#define DICT

/*
    - key-val pairs are stored in the nodes of one of many linked lists
    - linked lists are pointed to by the elements of an array
    - the array is indexed by the hash value of the keys
*/

typedef struct {
    node *next;
    char *key; 
    char *val;
} node;

/**
 * Adds a new key-val pair or update the value of an existing key
 * 
 * Returns a pointer to the key-val pair created/modified or NULL if there is no 
 * enough memory for a new entry
 **/
node* add(char *key, char *val);

/**
 *  Returns a pointer to key-val pair or NULL if 'key' is not found
 **/
node* lookup(char *key);

#endif
