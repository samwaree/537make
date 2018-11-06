#ifndef PROJ_UTILS_H
#define PROJ_UTILS_H

#include "linked_list.h"
#include "build_spec_repr.h"

void mallocCheck(void*);
Buildspec* find(Node*, char*);
int findDuplicates(Node*, char*);

#endif
