#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
		//printf("Child exited with %d status. PID is %d\n", status, pid);
		if(status != 0){
            fprintf(stderr, "%d: Error running command for target \"%s\".\n", line_number, target);
			exit(-1);
		}
	}
}

