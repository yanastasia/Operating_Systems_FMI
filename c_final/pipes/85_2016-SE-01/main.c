#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv){
	if (argc != 2){
		errx(1, "Invalid number of arguments");
	}

	char* file = argv[1];

	int fd[2];
	if(pipe(fd) == -1){
		errx(1, "Pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1){
		errx(2, "Fork failed");
	}

	if(pid == 0){
		close(fd[0]);
		if(dup2(fd[1], 1) == -1){
			errx(3, "1 Dup2 failed");
		}

		if(execlp("cat", "cat", file, (char*)NULL) == -1){
			errx(4, "Execlp failed");
		}
	} 

	close(fd[1]);
	if(dup2(fd[0], 0) == -1){
		errx(5, "2 Dup2 failed");
	}

	if(execlp("sort", "sort", (char*)NULL) == -1){
		errx(6, "Execlp failed");
	}
	
	close(fd[0]);
	return 0;
}

