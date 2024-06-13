#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, char **argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	int scl = open(argv[1], O_RDONLY);
	if(scl == -1){
		err(1, "Error while opening %s", argv[1]);
	}

	int sdl = open(argv[2], O_RDONLY);
	if(scl == -1){
		err(1, "Error while opening file %s", argv[2]);
	}

	int fd = open("high.sd1", O_CREAT | O_WRONLY, 0666);
	if(fd == -1){
		err(1, "Error while opening file high.sd1");
	}

	uint16_t buff;
	uint8_t level;
	int b_read;
	int b_written;
	while((b_read = read(scl, &level, sizeof(level))) > 0){
		b_read = read(sdl, &buff, sizeof(buff));
		if(b_read != sizeof(buff))
			err(1, "Error while reading");
		if(level == 1){
			if((b_written = write(fd, &buff, sizeof(buff))) != sizeof(buff))
				err(1, "Error while writing in high.sd1");
		}
	}

	close(fd);
	close(sdl);
	close(scl);

	exit(0);
}

