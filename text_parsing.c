#include "build_spec_repr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

const int BUFFSIZE = 1024;

void trim(char*);

void printError(int line_number, char* line) {
    fprintf(stderr, "%d: Invalid Line: \"%s\"\n", line_number, line);
}

/*
 * This method will conduct a build on the first build target and its
 * dependencies.
 */
Node* parseMakefile(FILE* make){
    char* buff = malloc(BUFFSIZE * sizeof(char));
    int line_number = 1;
    char* line = malloc(BUFFSIZE * sizeof(char));
    Buildspec* curr = NULL;
    Node* bs_list = createList();
	char c;
	int i = 0;
	while((c = fgetc(make)) != EOF){
        if (i == 0 && c == '#') {
            while((c = fgetc(make)) != '\n' && c != EOF);
            line_number++;
            continue;
        }
        if (i == 0 && c == '\n') {
            curr = NULL;
            line_number++;
            continue;
        }
		if(i < BUFFSIZE){
			if(c == '\n'){
				buff[i] = '\0';
                strncpy(line, buff, strlen(buff) + 1);
//printf("Line: %s\n", line);   
                if (strchr(line, ':') != NULL) {
                    char* token = strtok(buff, ":");
//printf("Target: %s\n", token);
                    int space_flag = 0;
                    for (int j = 0; j < (int) strlen(token); j++) {
                        if(token[j] == ' ' || token[j] == '\t') {
                            space_flag = 1;
                        }
                        if (space_flag && token[j] != ' ') {
                            printError(line_number, line);
                            exit(-1);
                        }
                    }
                    trim(token);
                    char* target = malloc((strlen(token) + 1) * sizeof(char));
                    strncpy(target, token, strlen(token) + 1);
                    curr = createBuildSpec(target);
                    token = strtok(NULL, " \t");
                    while (token != NULL) {
                        if (strchr(token, ':') != NULL) {
                            printError(line_number, line);
                            exit(-1);   
                        } 
                        char* dep = malloc((strlen(token) + 1) * sizeof(char));
                        strncpy(dep, token, strlen(token));
                        addDependency(curr, dep);
                        token = strtok(NULL, " \t");
                    }
                    append(bs_list, curr);
                } else { // Must be a command
                    if (curr == NULL) {
                        printError(line_number, line);
                        exit(-1);
                    }
                    if (buff[0] != '\t') {
                        printError(line_number, line);
                        exit(-1);
                    }
                    Buildspec* temp = getElement(bs_list, size(bs_list) - 1);
                    char* command = malloc((strlen(buff) + 1) * sizeof(char));
                    strncpy(command, buff, strlen(buff) + 1);
                    addCommand(temp, command);
                }
                line_number++;
                i = 0;
			}else{
				buff[i] = c;
				i++;
			}
		}else{
			// Put in the line number where it errored; keep count
			fprintf(stderr, "%d: Line exceeded buffer size.\n", line_number);
			exit(-1);
		}
	}

	return bs_list;
}


void trim(char* str) {
    for (int i = 0; i < (int) strlen(str); i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            str[i] = '\0';
            return;
        }
    }
}
