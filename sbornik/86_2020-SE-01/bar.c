#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void close_safe(int fd){
	int errno_ = errno;
	close(fd);
	errno = errno_;
}

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	const char* pipe_name = "./pipe.txt";
	const char* command = argv[1];

	const pid_t child = fork();
	if(child == -1){
		err(1, "Unable to fork");
	}

	if(child == 0){
		int fd;
		if((fd = open(pipe_name, O_RDONLY)) == -1){
			err(1, "Couldn't open %s", pipe_name);
		}

		if(dup2(fd, 0) == -1){
			close_safe(fd);
			err(1, "Couldn't dup2");
		}

		if(execl(command, command, (char*)NULL) == 1){
			close_safe(fd);
			err(1, "Couldn't exec");
		}
	}

	int status;
	if(wait(&status) == -1){
		err(1, "Couldn't wait for child process to finish");
	}

	if(!WIFEXITED(status)){
		err(1, "Child process didn't terminate normally");
	}

	if(WEXITSTATUS(status) != 0){
		err(1, "Exit status of child process not 0");
	}

	exit(0);
}

