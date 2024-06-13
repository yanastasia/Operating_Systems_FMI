#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>

int main(int argc, char** argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(1, "Unable to open %s", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if(fd2 == -1){
		err(1, "Unable to open %s", argv[2]);
	}

	uint16_t encoded;
	uint8_t byte;
	ssize_t b_read;

	while((b_read = read(fd1, &encoded, sizeof(encoded))) > 0){
		byte = 0;
		for(int i = 0; i < 8; ++i){
		//	uint16_t temp = ((encoded >> i) & 0x03);
		//	if(temp == 0x01) {
		//		byte |= (1 << i);
		//	}
		}
		if(write(fd2, &byte, sizeof(byte)) != sizeof(byte)){
			close(fd1);
			close(fd2);
			err(1, "Error while writing in %s", argv[2]);
		}
	}

	close(fd1);
	close(fd2);

	exit(0);
}

