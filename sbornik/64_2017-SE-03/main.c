#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>

struct triplet{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

int main(int argc, char** argv){
	if(argc!=4)
		errx(1,"Invalid number of arguments");

	const char* patch = argv[1];
	const char *f1 = argv[2];
	char* f2 = argv[3];

	struct stat st_patch;
	if ((stat(patch, &st_patch)) == -1)
		err(1, "Unable to stat %s", patch);

	struct triplet t;

	int fd = open(patch, O_RDONLY);
	if (fd == -1)
		err(1, "Unable to open %s", patch);

	int fd1 = open(f1, O_RDONLY);
	if(fd1 == -1)
		err(1, "Unable to open %s", f1);

	int fd2 = open(f2, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if(fd2 == -1)
		err(1, "Unable to open %s", f2);

	int bytes_read;
	uint8_t buffer[2048];
	int bytes_written;

	while ((bytes_read = read(fd1, &buffer, sizeof(buffer))) > 0){
		bytes_written = write(fd2, &buffer, bytes_read);
		if(bytes_written != bytes_read){
			errx(1, "Unable to read from file %s", f2);
		}
	}
	if(bytes_read == -1)
		err(1, "Unable to read from file %s", f1);

	uint8_t c;
	int read_c;

	while((bytes_read = read(fd, &t, sizeof(t))) == sizeof(t)) {
		if(lseek(fd2, t.offset, SEEK_CUR) == -1)
			err(1, "Error while lseeking file %s", f2);
		if((read_c = (fd2, &c, sizeof(c))) == -1){
			err(1, "Error while reading file %s", f2);
		}

		if(c == t.original){
			if((bytes_written = write(fd2, &t.new, sizeof(t.new))) != sizeof(t.new)){
				err(1, "Error while writing in file %s", f2);
			}
		}
	}
	if(bytes_read == -1)
		err(1, "Error while reading from file %s", patch);

	close(fd1);
	close(fd2);
	close(fd);

	exit(0);
}

