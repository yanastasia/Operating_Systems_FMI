#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char** argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments");
	}
	int fd[2];
	if(pipe(fd) == -1){
		err(2, "Pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1){
		err(3, "Fork failed");
	}

	if(pid == 0){
		close(fd[0]);
		if(dup2(fd[1], 1) == -1){
			err(4, "Dup2 failed");
		}

		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1){
			err(5, "Execlp failed");
		}
	}
	
	close(fd[1]);

	if(dup2(fd[0], 0) == -1){
		err(6, "Dup2 failed");
	}

	if(execlp("sort", "sort", (char*)NULL) == -1){
		err(7, "execlp failed");
	}

	return 0;
}
