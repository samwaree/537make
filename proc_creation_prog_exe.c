// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Executes commands for a given target.
 * If an error is received, prints the line number as well as the target.
 */
void runCommand(char** command, char* target, int line_number){
	pid_t pid;
	pid = fork();
	int status;
	
	if(pid == 0){ 
		if(execvp(command[0], command) < 0){
			exit(-1);
		}else{
			exit(0);
		}
	}else{
		wait(&status);
		if(status != 0){
			fprintf(stderr, "%d: Error running command for target \"%s\".\n", line_number, target);
			exit(-1);
		}
	}
}

