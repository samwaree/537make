#include "build_spec_repr.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

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
            FILE* file = fopen(target, "r");
            if (file == NULL) {
                fprintf(stderr, "%d: File does not exist: \"%s\"\n", getLine(bs), target);
                exit(-1);                
            }
            fclose(file);

            struct stat file_stats;
            if (stat(target, &file_stats) < 0) {
                fprintf(stderr, "Error: Unable to open file's stats: %s\n", target);
                exit(-1);
            }

            if (file_stats.st_mtime > dep_mod_date) {
                dep_mod_date = file_stats.st_mtime;
            }
        } else {
            time_t temp = postOrder(next, bs_list);
            if (temp > dep_mod_date) {
                dep_mod_date = temp;
            }
        }
    }
    
    int target_mod_date = 0;
    FILE* file = fopen(getTarget(bs), "r");
    if (file == NULL) {
        // Build the target
        // Essentially run all the commands
    } else {
        fclose(file);
        struct stat file_stats;
        if (stat(getTarget(bs), &file_stats) < 0) {
            fprintf(stderr, "Error: Unable to open file's stats: %s\n", getTarget(bs));
            exit(-1);
        }
        target_mod_date = file_stats.st_mtime;
        if (target_mod_date < dep_mod_date) {
            // Build target
        }
    }
    // If dep_mod_time > targ_mod_time build and set new targ_mod_time
    return target_mod_date;
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
