#include <stdio.h>
#include <stdlib.h>

void runCommand(char** command){
	pid_t pid;
	pid = fork();
	int status;

	
	if(pid == 0){
		printf("I am a child.");
		if(execvp(command[0], command) < 0){
			exit(-1);
		}else{
			exit(0);
		}
	}else{
		wait(&status);
		printf("I am a parent.");
		if(status == -1){
			exit(-1);
		}
	}
}

