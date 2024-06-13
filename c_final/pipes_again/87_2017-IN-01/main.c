#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

//cut -d: -f7 /etc/passwd | sort | uniq -c | sort

int main(void){
	int pfd1[2];
	if(pipe(pfd1) == -1){
		err(1, "pipe failed");
	}

	const pid_t pid1=fork();
	if(pid1 == -1){
		err(1, "fork failed");
	}
	if(pid1 == 0){
		close(pfd1[0]);
		if(dup2(pfd1[1], 1) == -1){
			err(1, "dup2 failed");
		}
		
		if(execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) == -1){
			err(1, "execlp failed");
		}
	}
	
	close(pfd1[1]);

	int pfd2[2];
	if(pipe(pfd2) == -1){
		err(1, "pipe failed");
	}

	const pid_t pid2=fork();
	if(pid2 == -1){
		err(1, "fork failed");
	}
	if(pid2 == 0){
		close(pfd2[0]);
		if(dup2(pfd1[0], 0) == -1){
			err(1, "dup2 failed");
		}
		if(dup2(pfd2[1], 1) == -1){
			err(1, "dup2 failed");
		}

		if(execlp("sort", "sort", (char*)NULL) == -1){
			err(1, "exelp failed");
		}
	}

	close(pfd1[0]);
	close(pfd2[1]);

	int pfd3[2];
	if(pipe(pfd3) == -1){
		err(1, "pipe failed");
	}

	const pid_t pid3=fork();
	if(pid3 == -1){
		err(1, "fork failed");
	}

	if(pid3 == 0){
		close(pfd3[0]);
		if(dup2(pfd2[0], 0) == -1){
			err(1, "dup2 failed");
		}
		if(dup2(pfd3[1], 1) == -1){
			err(2, "dup2 failed");
		}

		if(execlp("uniq", "uniq", "-c", (char*)NULL) == -1){
			err(1, "execlp failed");
		}
	}

	close(pfd2[0]);
	close(pfd3[1]);

	if(dup2(pfd3[0], 0) == -1){
		err(1, "dup2 failed");
	}

	if(execlp("sort", "sort", (char*)NULL) == -1){
		err(1, "execlp failed");
	}
	
	close(pfd3[0]);
	return 0;
}
