#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include "linked_list.h"
#include "build_spec_repr.h"

Buildspec* find(Node*, char*);
void runBuildspecs(Node*);

#endif
