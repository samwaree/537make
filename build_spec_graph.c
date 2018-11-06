// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware
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
    // A list of all the tokenized strings, used to get the size
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
    
    // NULL terminator for use with exec() functions
    args[size(temp_command)] = NULL;
    return args;
}

/*
* Runs the commands for a given target
*/
void buildTarget(Buildspec* bs) {
    Node* commands = getCommands(bs);
    int line_number = getLine(bs) + 1;
    for (int i = 0; i < size(commands); i++) {
        char** args = commandToArgs((char*) getElement(commands,i));
        runCommand(args, getTarget(bs), line_number);
        line_number++;
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

/*
* Runs a post order traversal on the build graph to run a bulid spec and all
* its dependencies. Returns 1 if the target was built, 0 otherwise.
*/
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

            struct stat dep_stats;
            if (stat(target, &dep_stats) < 0) {
                fprintf(stderr, "Error: Unable to open file's stats: %s\n", target);
                exit(-1);
            }
            // Sets dep_mod_date to the most recent modification date
            if (dep_stats.st_mtime > dep_mod_date) {
                dep_mod_date = dep_stats.st_mtime;
            }
        } else {
            int depBuilt = postOrder(next, bs_list);
            if (depBuilt) { // If a dependency was built, build target
                build = 1;
            }
        }
    }
    
    if (access(getTarget(bs), R_OK) < 0) { // If the target doesn't exist
        if (build || dep_mod_date > 0) {
            buildTarget(bs);
            return 1;
        }
    } else { // If the target exists
        struct stat target_stats;
        if (stat(getTarget(bs), &target_stats) < 0) {
            fprintf(stderr, "Error: Unable to open file's stats: %s\n", getTarget(bs));
            exit(-1);
        }
        time_t target_mod_date = target_stats.st_mtime;

        // Build if a dependency was build or target is out of date
        if (target_mod_date < dep_mod_date || build) {
            buildTarget(bs);
            return 1;
        }
    }
    return 0;
}

/*
* Checks for cycles, and then tries to build the specified target
*/
void runMakefile(FILE* make, char* target) {
    // Parses the makefile to get all the build specs		
    Node* bs_list = parseMakefile(make);
    for (int i = 0; i < size(bs_list); i++) {
        Buildspec* bs = getElement(bs_list, i);
	    checkCycles(bs, bs_list, NULL);
    }
    if(size(bs_list) == 0){
    	exit(0);
    }

    Buildspec* bs;
    if(target == NULL){
	    bs = getElement(bs_list, 0);
    } else {
	    bs = find(bs_list, target);
	    if(bs == NULL){
	        fprintf(stderr, "Error: Could not find target \"%s\"\n", target);
	        exit(-1);
	    }
    }
        
    int build = postOrder(bs, bs_list);
    if (build == 0) {
        printf("Target \"%s\" is up to date.\n", getTarget(bs));
    }
}
