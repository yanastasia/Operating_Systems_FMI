#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

void close_safe(int fd){
	int errno_ = errno;
	close(fd);
	errno = errno_;
}

int main(int argc, char ** argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	const char* pipe_name = "./pipe.txt";
	const char* fileName = argv[1];

	if(mkfifo(pipe_name, 0666) == -1){
		err(1, "Couldn't mkfifo");
	}
	
	if(child == 0){
		int fd;
		if((fd = open(pipe_name, O_WRONLY)) == -1){
			err(1, "Couldn't open %s", pipe_name);
		}

		if(dup2(fd, 1) == -1){
			close_safe(fd);
			err(1, "Couldn't dup2");
		}
	}

	int status;
	if(wait(&status) == -1){
		err(1, "Couldn't wait for child");
	}

	if(!WIFEXITED(status)){
		err(1, "Child process terminated abnormally");
	}

	if(WEXITSTATUS(status) != 0){
		err(1, "Exit status of child process is not 0");
	}

	exit(0);
}

