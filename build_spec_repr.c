#include <stdlib.h>

typedef struct StringNode{
    char* name;
    struct StringNode* next;
}StringNode;

// Represents a build specification for the makefile
typedef struct Buildspec {
    char* target;
    StringNode* dependencies;
    StringNode* commands;
    StringNode* last; // A pointer to the last StringNode in the commands list    
}Buildspec;

/*
* Creates a build specification for the given list containing the target
* and dependencies.
*/
Buildspec* createBuildSpec(StringNode* list) {
    Buildspec* bs = (Buildspec*) malloc(sizeof(Buildspec));
    bs->target = list->name;
    bs->dependencies = list->next;
    bs->commands = (StringNode*) malloc(sizeof(StringNode));
    bs->last = bs->commands;

    return bs;
}

/*
* Add a command to a build specification, and increments the last pointer
*/
void addCommand(Buildspec* bs, char* command) {
    bs->last->name = command;
    bs->last->next = (StringNode*) malloc(sizeof(StringNode));
    bs->last = bs->last->next;
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
StringNode* getDependencies(Buildspec* bs) {
    return bs->dependencies;
}

/*
* Returns a list of all commands for the given buildspec
*/
StringNode* getCommands(Buildspec* bs) {
    return bs->commands;
}
