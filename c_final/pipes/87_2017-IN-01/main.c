#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(void) { //since we don't have arguments
	int fd[2]; // first pipe
	if(pipe(fd) == -1){
		err(1, "Pipe failed");
	}

	const pid_t pid1 = fork();
	if(pid1 == -1){
		err(2, "Fork failed");
	}

	if(pid1==0){ //child process
		close(fd[0]); //we close the read end 
		//we want to redirect the stdout(1) of "cut -d ':' -f 7  /etc/passwd" to the std output of the pipe
		//dup2(dst, src)
		if(dup2(fd[1], 1) == -1) {
			err(3, "Dup2 failed");
		}
		
		if(execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) == -1) {
			err(4, "Execlp failed");
		}

	}else{
		//parent process
		close(fd[1]);
	}

	int fd2[2];
	if(pipe(fd2) == -1){
		err(1, "Pipe failed");
	}

	const pid_t pid2=fork();
	if(pid2 == -1){
		err(2, "Fork failed");
	}

	if(pid2 == 0){
		//child process
		//we are going to write in the pipe again, so we close for reading
		close(fd2[0]);
		//we want to read from the previous file and write to the current pipe, we do that using 2 dup2
		//we don't read from stdin, but from the first pipe 
		if(dup2(fd[0], 0) == -1){
			err(3, "Dup2 failed");
		}
		
		if(dup2(fd2[1], 1) == -1){
			err(3, "Dup2 failed");
		}

		if(execlp("sort", "sort", (char*)NULL) == -1){
			err(4, "Execlp failed");
		}

	}else{
		close(fd2[1]);
	}

	int fd3[2];
	if(pipe(fd3) == -1){
		err(1, "Pipe failed");
	}

	const pid_t pid3=fork();
	if(pid3==-1){
		err(2, "Fork failed");
	}

	if(pid3==0){
		close(fd3[0]);

		if(dup2(fd2[0], 0) == -1){
			err(3, "Dup2 failed");
		}

		if(dup2(fd3[1], 1) == -1){
			err(3, "Dup2 failed");
		}

		if(execlp("uniq", "uniq", "-c", (char*)NULL) == -1){
			err(4, "Execlp failed");
		}
	}else{
		close(fd3[1]);
	}

	//when we have the last pipe we don't create new file descriptors
	close(fd[0]);
	close(fd2[0]);

	if(dup2(fd3[0], 0) == -1){
		err(4, "Dup2 failed");
	}

	//we want the result to be on stdout

	if(execlp("sort", "sort", "-n", (char*)NULL) == -1){
		err(1, "Execlp failed");
	}

	close(fd3[0]);
	
	return 0;
}
