#include <stdio.h>
#include "build_spec_graph.h"

int main(int argc, char** argv) {
    // Test    
    FILE* make = fopen("Testmake", "r");
    runMakefile(make);
	return 0;
}
