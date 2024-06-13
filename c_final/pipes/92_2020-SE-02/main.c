#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char** argv){
	if(argc!=3){
		errx(1, "Invalid number of arguments");
	}

	int pf[2];
	if(pipe(pf) == -1){
		err(2, "Error while piping");
	}

	const pid_t pid = fork();
	if(pid == -1){
		err(3, "Error while forking");
	}
	if(pid == 0){
		close(pf[0]);
		dup2(pf[1], 1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1){
			err(4, "Execlp failed");
		}
	}

	close(pf[1]);
	int fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if(fd == -1){
		errx(1, "Failed to open");
	}

	char c;
	int rs;
	while((rs = read(pf[0], &c, sizeof(c))) == sizeof(c)){
		if(c==0x55){
			continue;
		}
		if(c==0x7D){
			if(read(pf[0], &c, sizeof(c)) == -1){
				err(5, "Error while reading");
			}
			c = c^0x20;
			if(write(fd, &c, sizeof(c)) != sizeof(c)){
				err(6, "Error while writing");
			}
		}
	}
	if(rs == -1){
		err(7, "Error while reading from pipe");
	}

	close(pf[0]);
	close(fd);
	return 0;
}

