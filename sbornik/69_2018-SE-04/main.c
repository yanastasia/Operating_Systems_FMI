#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

int fds[2];

void close_all(void){
	int errno_ = errno;
	for(int i = 0; i < 2; i++){
		if(fds[i] >= 0){
			close(fds[i]);
		}
	}
	
	errno = errno_;
}

int comp(const void* a, const void* b){
	const uint16_t* first = (const uint16_t*)a;
	const uint16_t* second = (const uint16_t*)b;

	if(*first < *second) {
		return -1;
	} else if(*first > *second){
		return 1;
	}

	return 0;
}


void load_arr(uint16_t* numbers, const char* fileName){
	uint16_t num;
	int bytes_count, ind = 0;
	while((bytes_count = read(fds[0], &num, sizeof(num))) == sizeof(num)){
		numbers[ind] = num;
		ind++;
	}
	
	if(bytes_count == -1){
		close_all();
		err(1, "Couldn't read from file");
	}

}

void load_file(uint16_t* numbers, int numbers_count, const char* fileName){
	for(int i = 0; i < numbers_count; ++i){
		if(write(fds[1], &numbers[i], sizeof(numbers[i])) == -1){
			close_all();
			err(1, "Could not write to file");
		}
	}
}


int open_safe(const char* fileName, int flags, mode_t mode){
	int fd;
	if((fd = open(fileName, flags, mode)) == -1) {
		close_all();
		err(1, "Could not open file");
	}

	return fd;
}

int main(int argc, char ** argv){
	if( argc != 3){
		errx(1, "Invalid number of arguments");
	}

	const char* f1 = argv[1];
	const char* f2 = argv[2];

	struct stat s;
	if(stat(f1, &s) == -1){
		err(1, "Couldn't stat");
	}

	if(s.st_size%2 != 0){
		err(1, "File doesn't contain uint16_t numbers only");
	}

	fds[1] = open_safe(f1, O_RDONLY, S_IRUSR | S_IWUSR);
	fds[2] = open_safe(f2, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	int numbers_count = s.st_size / sizeof(uint16_t);
	if(numbers_count > 65535){
		err(1, "Too many numbers in file");
	}

	uint16_t* numbers = malloc(numbers_count * sizeof(uint16_t));
	load_arr(numbers, f1);
	qsort(numbers, numbers_count, sizeof(uint16_t), comp);
	load_file(numbers, numbers_count, f2);
	close_all();
	free(numbers);
	exit(0);
}

