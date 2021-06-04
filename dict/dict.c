#include <string.h>
#include <stdlib.h>
#include "dict.h"

#define DICTSIZE 101

static node_t *dict[DICTSIZE];

// char *strdup(char *s)
// {    
//     char *p = (char *) malloc(strlen(s)+1); // +1 for '\0'
//     if (p != NULL)
//         strcpy(p, s);
//     return p;
// }

unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % DICTSIZE;
}

node_t* lookup(char *key) {
    node_t *np;
    
    for(np = dict[hash(key)]; np != NULL; np = np->next) {
        if(strcmp(np->key, key) == 0)
            return np;
    }
    return NULL;
}

node_t* add(char *key, char *val) {
    node_t *np;
    unsigned hashval;

    if((np = lookup(key)) == NULL){
        np = (node_t *) malloc(sizeof(*np));
        if (np == NULL || (np->key = strdup(key)) == NULL)
            return NULL;
        hashval = hash(key);

        //adding new node to the front of the linked list
        np->next = dict[hashval];
        dict[hashval] = np;

    } else {
        free((void*) np->val);
    }

    if((np->val = strdup(val)) == NULL)
        return NULL;
    return np;
}