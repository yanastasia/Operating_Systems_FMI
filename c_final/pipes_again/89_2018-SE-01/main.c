#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char** argv){
	if(argc != 2){
		err(1, "invalid number of arguments");
	}

	int pfd1[2];
	if(pipe(pfd1) == -1){
		err(1, "pipe failed");
	}

	const pid_t pid1 = fork();
	if(pid1 == -1){
		err(1, "fail");
	}
	if(pid1 == 0){
		close(pfd1[0]);
		if(dup2(pfd1[1], 1) == -1){
			err(1, "failed");
		}

		if(execlp("find", "find", argv[1], "-type", "f", "-printf", "%T@ %f\n", (char*)NULL) == -1){
			err(1, "execlp failed");
		}
	}

	close(pfd1[1]);

	int pfd2[2];
	if(pipe(pfd2) == -1){
		err(1, "Pipe failed");
	}

	const pid_t pid2 = fork();
	if(pid2 == -1){
		err(1, "failed");
	}
	if(pid2 == 0){
		close(pfd2[0]);
		if(dup2(pfd1[0], 0) == -1){
			err(1, "dup failed");
		}
		if(dup2(pfd2[1], 1) == -1){
			err(2, "dup2 failed");
		}

		if(execlp("sort", "sort", "-rnk1", (char*)NULL) == -1){
			err(1, "failed");
		}
	}
	close(pfd1[0]);
	close(pfd2[1]);

	int pfd3[2];
	if(pipe(pfd3) == -1){
		err(1, "failed");
	}

	const pid_t pid3 = fork();
	if(pid3 == -1){
		err(1, "failed");
	}
	if(pid3 == 0){
		close(pfd3[0]);
		if(dup2(pfd2[0], 0) == -1){
			err(1, "failed");
		}
		if(dup2(pfd3[1], 1) == -1){
			err(1, "failed");
		}

		if(execlp("head", "head", "-n1", (char*)NULL) == -1){
			err(1, "failed");
		}
	}
	close(pfd2[0]);
	close(pfd3[1]);

	if(dup2(pfd3[0], 0) == -1){
		err(1, "failed");
	}

	if(execlp("awk", "awk", "{print $2}", (char*)NULL) == -1){
		err(1, "failed");
	}
	close(pfd3[0]);
	return 0;
}
