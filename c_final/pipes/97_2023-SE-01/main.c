#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	int pd[2];
	if(pipe(pd) == -1){
		err(1, "pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1){
		err(3, "Fork failed");
	}

	if(pid == 0){
		close(pd[0]);
		if(dup2(pd[1], 1) == -1){
			err(4, "Dup2 failed");
		}

		if(execlp("find", "find", argv[1], "-type", "f", "!", "-regex", ".*\\.hash$", (char*)NULL) == -1){
			err(5, "execlp failed");
		}
	}

	close(pd[1]);

	if(dup2(pd[0], 0) == -1){
		err(6, "Dup2 failed");
	}

	if(execlp("xargs", "xargs", "-I", "{}", "sh", "-c", "md5sum \"{}\">\"{}.hash\"", (char*)NULL) == -1){
		err(7, "execlp failed");
	}

	close(pd[0]);

	return 0;
}
