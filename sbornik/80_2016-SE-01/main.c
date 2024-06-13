#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	int pf[2];

	if(pipe(pf) == -1)
		err(1, "Pipe failed");

	pid_t pid = fork();
	if(pid == -1)
		err(1, "Could not fork");

	if(pid == 0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
			err(1, "Execlp failed");
		}
		close(pf[1]);
	}
	if(pid > 0){
		close(pf[1]);
		dup2(pf[0], 0);
		wait(NULL);
		if(execlp("sort", "sort", (char*)NULL) == -1)
			err(1, "Execlp failed");
		close(pf[0]);
	}
}

