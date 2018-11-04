#include <stdio.h>
#include <stdlib.h>
#include "build_spec_repr.h"
#include "linked_list.h"

/*
 * MallocCheck evaluates pointers to make sure they were properly allocated.
 */
void mallocCheck(void* ptr){
	if(ptr == NULL){
		printf("Error: Memory not allocated.");
		exit(-1);
	}
}

/*
 * Finds and returns the Buildspec with the given target
 * Returns NULL if none found
 */
Buildspec* find(Node* bs_list, char* target) {
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
        if (strcmp(target, getTarget(bs)) == 0) {
            return bs;
        }
    }
    return NULL;
}
