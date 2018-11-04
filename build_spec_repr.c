#include <stdlib.h>
#include "linked_list.h"

// Represents a build specification for the makefile
typedef struct Buildspec {
    char* target;
    Node* dependencies;
    Node* commands;  
    int marked;
    int line_number;
}Buildspec;

/*
* Creates a build specification for the given target
*/
Buildspec* createBuildSpec(char* targ, int num) {
    Buildspec* bs = (Buildspec*) malloc(sizeof(Buildspec));
    bs->target = targ;
    bs->dependencies = createList();
    bs->commands = createList();
    bs->marked = 0;
    bs->line_number = num;
    return bs;
}

/*
* Add a command to a build specification, and increments the last pointer
*/
void addCommand(Buildspec* bs, char* command) {
    append(bs->commands, command);
}

void addDependency(Buildspec* bs, char* dep) {
    append(bs->dependencies, dep);
}
/*
* Returns the target for the given buildspec
*/
char* getTarget(Buildspec* bs) {
    return bs->target;
}

/*
* Returns a list of dependencies of the given buildspec
*/
Node* getDependencies(Buildspec* bs) {
    return bs->dependencies;
}

/*
* Returns a list of all commands for the given buildspec
*/
Node* getCommands(Buildspec* bs) {
    return bs->commands;
}

/*
* Returns the line number of the build spec
*/ 
int getLine(Buildspec* bs) {
    return bs->line_number;
}
/*
* Returns whether the Buildspec is marked
* Used for checking for cycles
*/
int isMarked(Buildspec* bs) {
    return bs->marked;
}

/*
* Marks the buildspec
*/
void mark(Buildspec* bs) {
    bs->marked = 1;
}

/*
* Unmarks the buildspec
*/
void unmark(Buildspec* bs) {
    bs->marked = 0;
}
