#include "build_spec_repr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int BUFFSIZE = 1000;

Buildspec* basicBuild();
Buildspec* fullBuild();
Buildspec* findBuild();

/*
 * This method will read in the Makefile and check that it exists. If it exists
 * it will check the command on the command line to see if it should call the
 * basicBuild method which indicates that 537make was the only command inputted
 * or if it should do a full build of the Makefile.
 */
Buildspec* textParse(FILE* make, char* target){
	if(make == NULL){
		printf("Error: Cannot open Makefile");
		exit(-1);
	}
	char* buff = (char*) malloc(BUFFSIZE * sizeof(char));
	if(buff == NULL){
		printf("Error: Cannot allocate memory.");
		exit(-1);
	}
	if(target == NULL){
		return basicBuild(buff, make);
	}else{
//		findBuild(buff, make, target);
	}
	return NULL;
}

/*
 * This method will conduct a build on the first build target and its
 * dependencies.
 */
Buildspec* basicBuild(char* buff, FILE* make){
	char* line;
	char c;
	int i = 0;
	while((c = fgetc(make)) != EOF){
        if (i == 0 && c == '#') {
            //Ignore rest of line
        }
		if(i < BUFFSIZE){
			if(c == '\n'){
				line = malloc(i * sizeof(char));
				line[i] = '\0';
                break;
			}else{
				buff[i] = c;
				i++;
			}
		}else{
			// Put in the line number where it errored; keep count
			printf("Error: Target size exceeded capacity.");
			exit(-1);
		}
	}
	if (line == NULL) {
        //Print error
    }

	i = 0;
	int j = 0;
	int d = 0;
	char* name;
	while(i < (int)strlen(line)){
		if(line[i] == ' '){
			// figure out later
			printf("Error:");
		}else if(line[i] != ':'){
			i++;
		}else{
			d = i - j;
			name = malloc(d * sizeof(char));
			while(j < i){
				name[j] = line[j];
				j++;
			}
			name[j] = '\0';
			break;
		}
	}
    if (name == NULL) {
        // Print error
        exit(-1);
    }

    Buildspec* bs = createBuildSpec(name);
	j++;
	i++; 
	while(j < (int)strlen(line)){
		if((i == j) && ((line[i] == ' ') || (line[j] == '\t'))){
			i++;
			j++;
		}else if((line[i] == ' ') || (line[i] == '\t')){
			d = i - j;
			name = malloc(d * sizeof(char));
			while(j < i){
				name[j] = line[j];
				j++;
			}
			name[j] = '\0';
			addDependency(bs, name);
		}
	}
    
    i = 0;

    // Get commands
    while (1) {
        c = fgetc(make);
        if (c != '\t') {
            // Print error
        }
        while ((c = fgetc(make)) != EOF) {
            if (i < BUFFSIZE) {
                if (c == '\n') {
                    buff[i] = '\0';
                    i = 0;
                    break;
                } else {
                    buff[i] = c;
                    i++;
                }    
            } else {
                //Print error
            }
        }
    }
	return bs;
}


/*
 * This method will be used to find build specs for the dependencies of a given
 * build target.
 */
//void* findBuild(char* buff, FILE* make, char* target){
//	return NULL;
//}


