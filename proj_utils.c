#include <stdio.h>

/*
 * MallocCheck evaluates pointers to make sure they were properly allocated.
 */
void* mallocCheck(void* ptr){
	if(ptr == NULL){
		fprint("Error: Memory not allocated.");
		exit(-1);
	}
}


