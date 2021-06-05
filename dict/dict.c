#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dict.h"

#define DICTSIZE 101

static node_t *dict[DICTSIZE];

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

bool delete(char *key) {

    node_t *curr, *prev = NULL;

    int hashval = hash(key);
    for(curr = dict[hashval]; curr != NULL; prev = curr, curr = curr->next) {
        if(strcmp(curr->key, key) == 0) {
            if(prev == NULL) {
                //remove node from front of list
                dict[hashval] = curr->next;                
            } else {                
                prev->next = curr->next;
            }
            free((void*) curr->key);
            free((void*) curr->val);
            free((void*) curr);
            return 0;
        }
    }

    return 1;
}

void print() {
    node_t *np;

    for (size_t i = 0; i < DICTSIZE; i++) {
        int has_elements = 0;
        for(np = dict[i]; np != NULL; np = np->next) {
            has_elements = 1;
            if(np == dict[i])
                printf("%zu ", i);
            printf("- (%s,%s) ", np->key, np->val);
        }
        if(has_elements)
            printf("\n");
    }
}