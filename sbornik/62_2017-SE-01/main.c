#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 4) {
		errx(1, "Expected 4 arguments");
	}

	const char* f1 = argv[1];
	const char* f2 = argv[2];
	struct stat s1;
	struct stat s2;

	if(stat(f1, &s1) == -1){
		err(1, "Error while stat file %s", f1);
	}

	if(stat(f2, &s2) == -1){
		err(1, "Error while stat filr %s", f2);
	}

	uint32_t size1 = s1.st_size;
	uint32_t size2 = s2.st_size;

	if(size1 != size2){
		errx(1, "Files are not consistent %s, %s", f1, f2);
	}
	
	int fd1 = open(f1, O_RDONLY);
	int fd2 = open(f2, O_RDONLY);

	if(fd1 == -1){
		err(1, "Error while opening file %s", f1);
	}

	if(fd2 == -1){
		close(fd1);
		err(1, "Error while opening file %s", f2);
	}

	const char* f3 = argv[3];
	int fd3 = open(f3, O_APPEND | O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

	if(fd3 == -1){
		close(fd1);
		close(fd2);
		err(1, "Error while opening file %s", f3);
	}

	ssize_t rd1 = -1;
	ssize_t rd2 = -1;
	
	struct triple_t{
 	   uint16_t offset;
 	    uint8_t b1;
 	    uint8_t b2;
 	}__attribute__((packed)) t;

	for(t.offset = 0; t.offset < size1; t.offset++){
		if((rd1 = read(fd1, &t.b1, sizeof(t.b1)) != sizeof(t.b1))){
			close(fd1);
			close(fd2);
			close(fd3);
			errx(1, "Error while reading file %s", f1);
		}

		if((rd2 = read(fd2, &t.b2, sizeof(t.b2))) != sizeof(t.b2)){
			close(fd1);
			close(fd2);
			close(fd3);
			errx(1, "Error while reading file %s", f2);
		}
		
		if(t.b1 != t.b2){
			ssize_t wr = write(fd3, &t, sizeof(t));
			if(wr == -1 || wr != sizeof(t)) {
				close(fd1);
				close(fd2);
				close(fd3);
				errx(1, "Error while writing in %s", f3);
			}
		}
	}
	
	if(rd1 == -1 || rd2 == -1) {
		close(fd1);
		close(fd2);
		close(fd3);
		errx(1, "Error while reading from %s and %s", f1, f3);
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}



