#include "build_spec_repr.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Buildspec* find(Node* bs_list, char* target) {
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
        if (strcmp(target, getTarget(bs)) == 0) {
            return bs;
        }
    }
    return NULL;
}

void checkCycles(Buildspec* bs, Node* bs_list) {
    if (isMarked(bs)) {
        fprintf(stderr, "Cycle Detected with target %s\n", getTarget(bs));
        exit(-1);  
    } else {
        mark(bs);
    }
    Node* deps = getDependencies(bs);
    for (int i = 0; i < size(deps); i++) {
        char* target = getElement(deps, i);
        Buildspec* next = find(bs_list, target);
        if (next != NULL) {
            checkCycles(next, bs_list);
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
void runBuildspecs(Node* bs_list) {
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
        checkCycles(bs, bs_list);
    }
}
