#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char ** argv){
	if(argc != 3) 
		errx(1, "Invalid number of arguments");

	int pf[2];
	if(pipe(pf) == -1){
		err(1, "Pipe failed");
	}

	pid_t pid = fork();
	
	if(pid == -1){
		err(1, "Fork failed");
	}

	if(pid == 0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1)
			err(1, "Execlp failed");
	}

	close(pf[1]);

	int fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if(fd == -1)
		err(1, "Error while opening");
	
	char c;
	int b_read;
	while((b_read = read(pf[0], &c, sizeof(c))) == sizeof(c)){
		if(c == 0x7D){
			if(read(pf[0], &c, sizeof(c)) == -1)
				err(1, "Error while reading");
			c = c^0x20;
			if(write(fd, &c, sizeof(c)) != sizeof(c)){
				err(1, "Error while writing");
			}
		}
		if(c == 0x55)
			continue;
	}
	if(b_read == -1)
		err(1, "Error while reading");
	
	close(pf[0]);
	close(fd);
	exit(0);
}


