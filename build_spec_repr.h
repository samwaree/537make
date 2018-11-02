#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H

#include "linked_list.h"

typedef struct Buildspec Buildspec;

Buildspec* createBuildSpec(char*);

void addCommand(Buildspec*, char*);
void addDependency(Buildspec*, char*);

char* getTarget(Buildspec*);
Node* getDependencies(Buildspec*);
Node* getCommands(Buildspec*);

#endif
