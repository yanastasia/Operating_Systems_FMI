#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <err.h>

//find $dir -type f --printf '%T@ %p' | sort -n -k 1 | tail -n 1

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Pipe failed");
	}

	int pf[2];
	if(pipe(pf) == -1)
		err(1, "Pipe failed");

	pid_t pid = fork();
	if(pid == 0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("find", "find", argv[1], "-type", "f", "-printf", "%T@ %p\n", (char*)NULL) == -1){
			err(1, "execlp failed");
		}
	}

	int pf2[2];
	if(pipe(pf2) == -1){
		err(1, "Pipe failed");
	}
	pid_t pid2 = fork();
	if(pid2 == -1)
		err(1, "Fork failed");

	if(pid2 == 0){
		close(pf[1]);
		close(pf2[0]);
		dup2(pf[0],0);
		dup2(pf[1],1);
		if(execlp("sort", "sort", "-n", "-k1", (char*)NULL) == -1)
			err(1, "execlp failed");
	}
	close(pf[0]);
	close(pf[1]);
	close(pf2[1]);

	dup2(pf2[0], 0);

	if(execlp("tail", "tail", "-n1", (char*)NULL) == -1)
		err(1, "execlp failed");
}


