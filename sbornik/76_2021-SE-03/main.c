#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char ** argv){
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

	struct stat st;
	if(stat(argv[1], &st) == -1){
		err(1, "Stat failure");
	}
	
	uint32_t size = st.st_size / sizeof(uint16_t);

	if(size > 524288){
		err(1, "Too many elements in file %s", argv[1]);
	}

	int b_read;
	uint16_t buff;
	dprintf(fd2, "#include <stdint.h>\n");
	dprintf(fd2, "uint32_t arrN = %d;\n", size);
	dprintf(fd2, "uint16_t arr[] = {");

	while((b_read = read(fd1, &buff, sizeof(buff))) > 0){
		dprintf(fd2, "%d,", buff);
	}
	if(b_read == -1){
		err(1, "Unable to read from %s", argv[1]);
	}

	dprintf(fd2, "};");

	close(fd2);
	close(fd1);

	exit(0);
}
