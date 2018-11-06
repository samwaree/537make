// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "build_spec_graph.h"

/*
 * The main method initializes the execution of makefile commands dependent on user input.
 */
int main(int argc, char** argv) {
    int option;
    char* target_makefile = NULL; // Keeps track of a makefile if '-f' is specified.
    // Checks to see if '-f' followed by a target makefile was specified by user.
    while ((option = getopt(argc, argv, ":f:")) != -1) {
        switch(option) {
            case 'f':
                target_makefile = optarg;
                break;
            case ':':
                printf("Usage: 537make <target> [-f Name of makefile]\n");
                exit(-1);
            case '?':
                printf("Usage: 537make <target> [-f Name of makefile]\n");
                exit(-1);        
        }
    } 
    FILE* make; // File pointer for the makefile being used.
    // If no makefile has been specified, check to see if one exists in the current working directory.
    if (target_makefile == NULL) {
        if((make = fopen("Makefile", "r")) == NULL){
		    if((make = fopen("makefile", "r")) == NULL){
			    fprintf(stderr, "Error: Could not find Makefile.\n");
			    exit(-1);
		    }	
	    }
    // If a makefile has been specified, check to see if it exists before reading through the makefile.
    } else {
        if ((make = fopen(target_makefile, "r")) == NULL) {
            fprintf(stderr, "Error: Could not find Makefile \"%s\"\n", target_makefile);
            exit(-1);
        }
    }
    // If no makefile has been specified, check to see if a target has been specified to run its build commands.
    if(target_makefile == NULL){
        if (argc == 2) {
	    runMakefile(make, argv[1]);
        }else if(argc == 1){
	    runMakefile(make, NULL);
        } else{
	    printf("Usage: 537make <target> [-f Name of Makefile]\n");
	    exit(-1);
	}
    // If a makefile has been specified, check to see if a target has been specified to run its build commands.
    } else {
        if (argc == 4) {
            runMakefile(make, argv[3]);
        } else if (argc == 3) {
            runMakefile(make, NULL);
        } else {
            printf("Usage: 537make <target> [-f Name of Makefile]\n");
            exit(-1);
        }
    }
    fclose(make);
    return 0;
}
