#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char** argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments");
	}

	const char* fifo = "/tmp/86fifo";
	if(mkfifo(fifo, 0666) == -1){
		errx(2, "Fifo failed");
	}

	int fd = open(fifo, O_WRONLY);
	if(fd == -1){
		errx(3, "Open failure");
	}

	dup2(fd, 1);

	if(execlp("cat", "cat", argv[1], (char*) NULL) == -1){
		errx(4, "Execlp failed");
	}
}

