#include <stdlib.h>
#include "linked_list.h"

typedef struct StringNode{
    char* name;
    struct StringNode* next;
}StringNode;

// Represents a build specification for the makefile
typedef struct Buildspec {
    char* target;
    Node* dependencies;
    Node* commands;  
}Buildspec;

/*
* Creates a build specification for the given target
*/
Buildspec* createBuildSpec(char* targ) {
    Buildspec* bs = (Buildspec*) malloc(sizeof(Buildspec));
    bs->target = targ;
    bs->dependencies = createList();
    bs->commands = createList();

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
