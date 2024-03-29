// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware
#include "build_spec_repr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "proj_utils.h"

const int BUFFSIZE = 1024;

void trim(char*);

/*
* Prints out the standard error statement
*/ 
void printError(int line_number, char* line) {
    fprintf(stderr, "%d: Invalid Line: \"%s\"\n", line_number, line);
}

/*
* This method parses the whole Makefile and creates a linked list of build
* specifications 
*/
Node* parseMakefile(FILE* make){

    char* buff = malloc(BUFFSIZE * sizeof(char));
    mallocCheck(buff);
    char* line = malloc(BUFFSIZE * sizeof(char));
    mallocCheck(line);

    int line_number = 1;

    // Represents the current Build spec to add commands to
    Buildspec* curr = NULL;

    // The list of build specs in the makefile
    Node* bs_list = createList();

	char c;
	int i = 0;
	while((c = fgetc(make)) != EOF){
        // If there is a null byte, exit
        if (c == '\0') {
            fprintf(stderr, "%d: Invalid character: \'\\0\'.\n", line_number);
            exit(-1);
        }
        // Ignores commented lines
        if (i == 0 && c == '#') {
            while((c = fgetc(make)) != '\n' && c != EOF) { //Flushes line while checking
                if (c == '\0') {                           //for bad input 
                    fprintf(stderr, "%d: Invalid character: \'\\0\'.\n", line_number);
                    exit(-1);
                }
                if (i >= BUFFSIZE) {
                    fprintf(stderr, "%d: Line exceeded buffer size.\n", line_number);
                    exit(-1);
                }
                i++;
            }
            i = 0;
            line_number++;
            continue;
        } else if (c == '#') { // A bad comment
            fprintf(stderr,"%d: Invalid character: \'#\'.\n", line_number);
            exit(-1);
        }
        // Ignores empty lines
        if (i == 0 && c == '\n') {
            line_number++;
            continue;
       }
        
        //Reads in lines less than BUFFSIZE
		if(i < BUFFSIZE){
			if(c == '\n'){ //Once we have a complete line
				buff[i] = '\0';

                // Copies buff to line for error printing
                strncpy(line, buff, strlen(buff) + 1);
            
                if (strchr(buff, ':') != NULL) { // Treat the line as a target
                    // Checks for an empty target
                    if (buff[0] == ':') {
                        printError(line_number, line);
                        exit(-1);
                    }
                    // Split the line by a : to get target name
                    char* token = strtok(buff, ":");
                    int space_flag = 0;
                        
                    // Makes sure that the target doesn't being with space                    
                    if (token[0] == ' ' || token[0] == '\t') {
                        printError(line_number, line);
                        exit(-1);
                    }

                    // Makes sure there is only one target before the :
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
                    // Copies the token to a new char* and creates a new bs
                    char* target = malloc((strlen(token) + 1) * sizeof(char));
                    mallocCheck(target);
                    strncpy(target, token, strlen(token) + 1);

                    // Makes sure target isn't duplicate
                    if (find(bs_list, target) != NULL) {
                        printError(line_number, line);
                        exit(-1);
                    }
                    curr = createBuildSpec(target, line_number);

    
                    token = strtok(NULL, " ");
                    while (token != NULL) { // Gets all the dependencies left
			        if ((strchr(token, ':') != NULL) || (strchr(token, '\t') != NULL)){
                        printError(line_number, line);
                        exit(-1);   
                    } 
//			// Checks for a duplicate dependency
//			if (findDuplicates(getDependencies(curr), token) == 1) {
//			    printError(line_number, line);
//			    exit(-1);
//			}
			// Copies the token to a new char* and add to curr
 			char* dep = malloc((strlen(token) + 1) * sizeof(char));
                        mallocCheck(dep);
			memset(dep, 0, (strlen(token) + 1) * sizeof(char));
                        strncpy(dep, token, strlen(token));
                        addDependency(curr, dep);

                        token = strtok(NULL, " ");
                    }
                    append(bs_list, curr);
                } else { // Must be a command
                    if (curr == NULL) { // If there is a line with no target
                        printError(line_number, line);
                        exit(-1);
                    }
                    if (buff[0] != '\t') { // Makes sure commands start with a tab
                        printError(line_number, line);
                        exit(-1);
                    }
                    Buildspec* temp = getElement(bs_list, size(bs_list) - 1);
                    char* command = malloc((strlen(buff) + 1) * sizeof(char));
                    mallocCheck(command);
		    memset(command, 0, (strlen(buff) + 1) * sizeof(char));
                    strncpy(command, buff, strlen(buff) + 1);
                    addCommand(temp, command);
                }
                line_number++;
                i = 0;
			} else { // Keep reading line
				buff[i] = c;
				i++;
			}
		}else{
			fprintf(stderr, "%d: Line exceeded buffer size.\n", line_number);
			exit(-1);
		}
	}

	free(buff);
	free(line);
	return bs_list;
}

/*
* Trims spaces off the end of a string
*/
void trim(char* str) {
    for (int i = 0; i < (int) strlen(str); i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            str[i] = '\0';
            return;
        }
    }
}
