#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "build_spec_graph.h"

int main(int argc, char** argv) {
    int option;
    char* target_makefile = NULL;
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
	FILE* make;
    if (target_makefile == NULL) {
        if((make = fopen("Makefile", "r")) == NULL){
		    if((make = fopen("makefile", "r")) == NULL){
			    fprintf(stderr, "Error: Could not find Makefile.\n");
			    exit(-1);
		    }	
	    }
    } else {
        if ((make = fopen(target_makefile, "r")) == NULL) {
            fprintf(stderr, "Error: Could not find Makefile \"%s\"\n", target_makefile);
            exit(-1);
        }
    }
	if(target_makefile == NULL){
        if (argc == 2) {
		    runMakefile(make, argv[1]);
        }else if(argc == 1){
		    runMakefile(make, NULL);
        } else{
		    printf("Usage: 537make <target> [-f Name of Makefile]\n");
		    exit(-1);
	    }
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
