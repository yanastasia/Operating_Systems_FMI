#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(argc, char** argv){
	if(argc != 2){
		errx("Invalid number of arguments");
	}
	
	const char* fileName = argv[1];
	
	int pf[2];
	if(pipe(pf) == -1){
		err(1, "Pipe failed");
	}


	const pid_t pid = fork();
	if(pid == -1){
		err(1, "Fork failed");
	}

	if(pid == 0){
		close(pf[0]);
		dup2(pf[1], 1);
		if(execlp("cat", "cat", fileName, (char*)NULL) == -1){
			err(1, "execlp failed");
		}

		close(pf[1]);
	}
	else{
		close(pf[1]);
		dup2(pf[0], 0);
		wait(NULL);
		if(execlp("sort", "sort", (char*)NULL) == -1){
			err(1, "Execlp failed");
		}
		close(pf[0]);
	}

	exit(0);
}


	
