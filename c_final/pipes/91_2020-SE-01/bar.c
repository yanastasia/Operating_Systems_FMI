#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>

int main(int argc, char** argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments");
	}

	int fd = open("/tmp/84fifo", O_RDONLY);
	if(fd == -1){
		errx(2, "Failed open");
	}

	dup2(fd, 0);
	if(execlp(argv[1], argv[1], (char*)NULL) == -1){
		err(3, "Execlp failed");
	}
	return 0;
}

