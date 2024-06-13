#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char** argv){
	//find argv[1] -type f -printf '%T@ %f\n' | sort -r | head -n 1 | awk '{print $2}'
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	int fd1[2];
	if(pipe(fd1) == -1){
		errx(2, "Pipe failed");
	}

	const pid_t pid1 = fork();
	if(pid1 == -1){
		errx(3, "Fork failed");
	}

	if(pid1 == 0){
		close(fd1[0]);
		if(dup2(fd1[1], 1) == -1){
			errx(4, "Dup2 failed");
		}

		if(execlp("find", "find", argv[1], "-type", "f", "-printf", "'%T@ %f\n'", (char*)NULL) == -1){
			errx(5, "Execlp failed");
		}
	}else{
		close(fd1[1]);
	}

	int fd2[2];
	if(pipe(fd2) == -1){
		errx(6, "Pipe failed");
	}

	const pid_t pid2 = fork();
	if(pid2 == -1){
		errx(7, "Fork failed");
	}

	if(pid2 == 0){
		close(fd2[0]);
		
		if(dup2(fd1[0], 0) == -1){
			errx(8, "Dup2 failed");
		}

		if(dup2(fd2[1], 1) == -1){
			errx(9, "Dup2 failed");
		}

		if(execlp("sort", "sort", "-r", (char*)NULL) == -1){
			errx(10, "Execlp failed");
		}
	}else{
		close(fd2[1]);
	}

	int fd3[2];
	if(pipe(fd3) == -1){
		errx(11, "Pipe failed");
	}

	const pid_t pid3 = fork();
	if(pid3 == -1){
		errx(12, "Fork failed");
	}

	if(pid3 == 0){
		close(fd3[0]);

		if(dup2(fd2[0], 0) == -1){
			errx(13, "Dup2 failed");
		}

		if(dup2(fd3[1], 1) == -1){
			errx(14, "Dup2 failed");
		}

		if(execlp("head", "head", "-n1", (char*)NULL) == -1){
			errx(15, "Execlp failed");
		}
	}else{
		close(fd3[1]);
	}

	close(fd1[0]);
	close(fd2[0]);
	
	if(dup2(fd3[0], 0) == -1){
		errx(16, "Dup2 failed");
	}

	if(execlp("awk", "awk", "{print $2}", (char*)NULL) == -1){
		errx(17, "Execlp failed");
	}

	close(fd3[0]);
	return 0;
}
