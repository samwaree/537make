#include <stdio.h>
#include <stdlib.h>
#include "build_spec_graph.h"

int main(int argc, char** argv) {
	FILE* make;
        if((make = fopen("Makefile", "r")) == NULL){
		if((make = fopen("makefile", "r")) == NULL){
			fprintf(stderr, "Error: Could not find Makefile.\n");
			exit(-1);
		}	
	}
	if(argc == 2){
		runMakefile(make, argv[1]);
	}else if(argc == 1){
		runMakefile(make, NULL);
	}else{
		printf("Usage: 537make <target>\n");
		exit(0);
	}
	fclose(make);
	return 0;
}
