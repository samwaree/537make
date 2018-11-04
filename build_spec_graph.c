#include "build_spec_repr.h"
#include "linked_list.h"
#include "text_parsing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "proc_creation_prog_exe.h"
#include "proj_utils.h"

char** commandToArgs(char* command) {
    Node* temp_command = createList();
    char* token = strtok(command, " \t");
    while(token != NULL) {
        append(temp_command, token);
        token = strtok(NULL, " \t");
    }

    char** args = (char**) malloc((size(temp_command) + 1) * sizeof(char*));

    for (int i = 0; i < size(temp_command); i++) {
        args[i] = getElement(temp_command, i);
    }
    
    args[size(temp_command)] = NULL;
    return args;
}

void buildTarget(Buildspec* bs) {
    Node* commands = getCommands(bs);
    for (int i = 0; i < size(commands); i++) {
        char** args = commandToArgs((char*) getElement(commands,i));
        runCommand(args);
    }
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
    for (int i = 0; i < size(deps); i++) {
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
        buildTarget(bs);
    } else {
        fclose(file);
        struct stat file_stats;
        if (stat(getTarget(bs), &file_stats) < 0) {
            fprintf(stderr, "Error: Unable to open file's stats: %s\n", getTarget(bs));
            exit(-1);
        }
        target_mod_date = file_stats.st_mtime;
        if (target_mod_date < dep_mod_date) {
            buildTarget(bs);
            target_mod_date = 0; 
        }
    }
    // If dep_mod_time > targ_mod_time build and set new targ_mod_time
    return target_mod_date;
}

/*
* Checks for cycles, and then runs the specificed Buildspec
*/
void runMakefile(FILE* make) {
    Node* bs_list = parseMakefile(make);
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
        checkCycles(bs, bs_list, NULL);
    }
    Buildspec* bs = getElement(bs_list, 0);
    
    time_t target_mod_date = 0;
    FILE* file;
    if ((file = fopen(getTarget(bs), "r")) != NULL) {

        fclose(file);

        struct stat file_stats;
        if (stat(getTarget(bs), &file_stats) < 0) {
            // Print error
            exit(-1);
        }
        target_mod_date = file_stats.st_mtime;
        time_t compare = postOrder(bs, bs_list);
        if (compare == target_mod_date) {
            printf("Target \"%s\" is up to date.\n", getTarget(bs));
            exit(1);
        }
    } else {
        postOrder(bs, bs_list);
    }
}
