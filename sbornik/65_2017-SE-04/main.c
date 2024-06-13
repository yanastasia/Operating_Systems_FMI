#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>

void read_fd(int fd){
	uint8_t sym;
	int bytes_count = -1;
	while((bytes_count = read(fd, &sym, sizeof(sym)))>0){
		if((write(1, &sym, sizeof(sym))) == -1){
			err(1, "Error while writing");
		}
	}

	if (bytes_count == -1){
		err(1, "Ubavle to read from file");
	}
	close(fd);
}


int main(int argc, char** argv){
	if(argc == 1){
		read_fd(0);
	}else{
		for(int i = 1; i < argc; ++i){
			int fd;
			if(strcmp(argv[i], "-") == 0){
				fd = 0;
			}
			else{
				fd = open(argv[i], O_RDONLY);
				if(fd == -1){
					err(1, "Error while openinf file %s", argv[i]);
				}
			}
			read_fd(fd);
		}
	}
	exit(0);
}




