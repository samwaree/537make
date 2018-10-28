#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H
typedef struct buildspec buildspec;

buildspec* createBuildSpec(char**);

void addCommand(char*);

char* getTarget(buildspec*);
char** getDependencies(buildspec*);
char** getCommands(buildspec*);

#endif
