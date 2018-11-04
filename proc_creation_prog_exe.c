#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void runCommand(char** command){
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
		//printf("Child exited with %d status.\n", status);
		if(status == -1){
            fprintf(stderr, "Unable to run command\n");
			exit(-1);
		}
	}
}

