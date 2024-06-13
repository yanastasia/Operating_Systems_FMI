#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

struct s{
	uint16_t magic;
	uint16_t fileType;
	uint32_t count;
} s;

int fds[5] = {-1,-1,-1,-1,-1};

void close_all(void){
	int old_errno = errno;
	for(size_t i = 0; i < 5; ++i){
		if(fds[i] != -1){
			close(fds[i]);
		}
	}

	errno = old_errno;
}


void read_safe(int fd, void* toWhere, ssize_t howMany, const char* fileName){
	int b_read = read(fd, toWhere, howMany);

	if(b_read < 0){
		close_all();
		err(1, "Couldn;t read from file %s", fileName);
	}

	if(b_read != howMany){
		close_all();
		errx(1, "Couldn't read all data from %s", fileName);
	}
}

void write_safe(int fd, void* what, ssize_t howMany, const char* fileName){
	int b_write = write(fd, what, howMany);

	if(b_write < 0){
		close_all();
		err(1, "Couldn't write to file %s", fileName);
	}

	if(b_write != howMany){
		close_all();
		errx(1, "Coulnd't write all data to %s", fileName);
	}
}

int open_safe(const char* file, int oflag){
	int fd = open(file, oflag);
	if(fd == -1){
		close_all();
		err(1, "Couldn't open file %s", file);
	}

	return fd;
}

int open_safe_create(const char* file){
	int fd = open(file, O_CREAT | O_WRONLY | O_RDONLY, S_IRUSR | S_IWUSR);
	if(fd == -1){
		close_all();
		err(1, "Couldn't open %s", file);
	}

	return fd;
}

void lseek_safe(int fd, size_t pos){
	if(lseek(fd, pos, SEEK_SET) < 0){
		close_all();
		err(1, "Failed at lseeking");
	}
}

int main(int argc, char ** argv){
	if(argc != 4){
		errx(1, "Invalid number of arguments");
	}

	fds[0] = open_safe(argv[1], O_RDONLY);
	fds[1] = open_safe(argv[2], O_RDONLY);
	fds[2] = open_safe_create(argv[3]);
	
	struct s hlist, hdata, hout;
	read_safe(fds[0], &hlist, sizeof(hlist), argv[1]);
	read_safe(fds[1], &hdata, sizeof(hdata), argv[2]);

	size_t headerLen = sizeof(s);
	const uint16_t magic_check = 0x5A4D;

	if(hlist.magic != magic_check || hdata.magic != magic_check){
		close_all();
		errx(1, "Files not in format");
	}

	struct stat slist, sdata;

	if(fstat(fds[0], &slist) == -1 || fstat(fds[1], &sdata) == -1){
		close_all();
		err(1, "Couldn't stat");
	}

	if((slist.st_size - headerLen) / sizeof(uint16_t) != hlist.count || (sdata.st_size - headerLen) / sizeof(uint32_t) != hdata.count){
		close_all();
		errx(1, "File length and elements count don't match");
	}

	if(hlist.count > hdata.count){
		close_all();
		err(1, "No pos in data");
	}

	uint16_t maxValue = 0;
	uint16_t v;

	for(size_t i = 0; i<hlist.count; ++i){
		read_safe(fds[0], &v, sizeof(v), argv[1]);
		if(v>maxValue){
			maxValue = v;
		}
	}
	
	hout.count = maxValue;
	hout.magic = magic_check;
	hout.fileType = 3;

	write_safe(fds[2], &hout, sizeof(hout), argv[3]);
	uint64_t dummy = 0;

	for(size_t i = 0; i < maxValue; ++i){
		write_safe(fds[2], &dummy, sizeof(dummy), argv[3]);
	}
	
	size_t pos = 0;
	uint16_t value;
	uint32_t data;
	uint64_t toWrite;

	while(pos < hlist.count){
		lseek_safe(fds[0], headerLen + pos * sizeof(uint16_t));
		read_safe(fds[0], &value, sizeof(value), argv[1]);

		lseek_safe(fds[1], headerLen + pos * sizeof(uint32_t));
		read_safe(fds[1], &data, sizeof(data), argv[2]);

		toWrite = data;
		lseek_safe(fds[2], headerLen + value * sizeof(uint64_t));
		write_safe(fds[2], &toWrite, sizeof(toWrite), argv[3]);

		pos++;
	}

	exit(0);
}

	
