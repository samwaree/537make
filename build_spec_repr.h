#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H
typedef struct Buildspec Buildspec;

typedef struct StringNode {
    char* name;
    struct StringNode* next;
}StringNode;

Buildspec* createBuildSpec(StringNode*);

void addCommand(Buildspec*, char*);

char* getTarget(Buildspec*);
StringNode* getDependencies(Buildspec*);
StringNode* getCommands(Buildspec*);

#endif
