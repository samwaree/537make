#include "build_spec_repr.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

/*
* Depth-first search to find cycles in the Buildspec graph
* Checks to see if there a exists a back path on a single ancestor tree
*/
void checkCycles(Buildspec* bs, Node* bs_list, Buildspec* prev) {
    if (isMarked(bs)) {
        fprintf(stderr, "Error: Cycle Detected:  %s <- %s\n", getTarget(bs), getTarget(prev));
        exit(-1);  
    } else {
        mark(bs);
    }
    Node* deps = getDependencies(bs);
    for (int i = 0; i < size(deps); i++) {
        char* target = getElement(deps, i);
        Buildspec* next = find(bs_list, target);
        if (next != NULL) {
            checkCycles(next, bs_list, bs);
        }
    }
    unmark(bs);
}

// Should do a post order traversal and run the commands
time_t postOrder(Buildspec* bs, Node* bs_list) {
    Node* deps = getDependencies(bs);
    time_t dep_mod_date = 1;
    for (int i = 0; i < size(bs_list); i++) {
        char* target = getElement(deps, i);
        Buildspec* next = find(bs_list, target);
        if (next == NULL) {
            // Check to see if its a file
            // If it is, set dep_mod_date if it is larger
        } else {
            time_t temp = postOrder(next, bs_list);
        }
    }
    time_t targ_mod_date = 0; // Use stat to get target mod date

    // If dep_mod_time > targ_mod_time build and set new targ_mod_time
    return targ_mod_date;
}

/*
* Checks for cycles, and then runs the specificed Buildspec
*/
void runBuildspecs(Node* bs_list) {
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
        checkCycles(bs, bs_list, NULL);
    }
}
