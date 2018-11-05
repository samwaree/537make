#include "build_spec_repr.h"
#include "linked_list.h"
#include "text_parsing.h"
#include "proc_creation_prog_exe.h"
#include "proj_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

/*
* Converts a command to a tokenized list of strings
*/ 
char** commandToArgs(char* command) {
    Node* temp_command = createList();
    char* token = strtok(command, " \t");
    while(token != NULL) {
        append(temp_command, token);
        token = strtok(NULL, " \t");
    }
    
    char** args = (char**) malloc((size(temp_command) + 1) * sizeof(char*));
    mallocCheck(args);

    for (int i = 0; i < size(temp_command); i++) {
        args[i] = getElement(temp_command, i);
    }
    
    args[size(temp_command)] = NULL;
    return args;
}

/*
* Runs the commands for a given target
*/
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
int postOrder(Buildspec* bs, Node* bs_list) {
    Node* deps = getDependencies(bs);
    if (size(deps) == 0) {
        buildTarget(bs);
        return 1;
    }
    
    int build = 0; // Changes to 1 if any dependencies were built
    time_t dep_mod_date = 0;
    for (int i = 0; i < size(deps); i++) {
        char* target = getElement(deps, i);
        Buildspec* next = find(bs_list, target);
        if (next == NULL) { // This means that the dep is a file
            if (access(target, R_OK) < 0) {
                fprintf(stderr, "%d: File does not exist: \"%s\"\n", getLine(bs), target);
                exit(-1);                
            }

            struct stat file_stats;
            if (stat(target, &file_stats) < 0) {
                fprintf(stderr, "Error: Unable to open file's stats: %s\n", target);
                exit(-1);
            }

            if (file_stats.st_mtime > dep_mod_date) {
                dep_mod_date = file_stats.st_mtime;
            }
        } else {
            int temp = postOrder(next, bs_list);
            if (temp) {
                build = 1;
            }
        }
    }
    
    if (access(getTarget(bs), R_OK) < 0) {
        if (build || dep_mod_date > 0) {
            buildTarget(bs);
            return 1;
        }
    } else {
        struct stat file_stats;
        if (stat(getTarget(bs), &file_stats) < 0) {
            fprintf(stderr, "Error: Unable to open file's stats: %s\n", getTarget(bs));
            exit(-1);
        }
        time_t target_mod_date = file_stats.st_mtime;
        if (target_mod_date < dep_mod_date || build) {
            buildTarget(bs);
            return 1;
        }
    }
    return 0;
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
    
    int build = postOrder(bs, bs_list);
    if (build == 0) {
        printf("Target \"%s\" is up to date.\n", getTarget(bs));
    }
}
