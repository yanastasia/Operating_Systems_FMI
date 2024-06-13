#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/param.h>
#include <errno.h>

int fds[3];

void close_all(void){
	int errno_ = errno;
	for (int i = 0; i < 3; ++i){
		if(fds[i] >= 0){
			close(fds[i]);
		}
	}

	errno = errno_;
}

int compareUint32_t(const void* a, const void* b){
	const uint32_t* num1 = (const uint32_t*)a;
	const uint32_t* num2 = (const uint32_t*)b;

	if(*num1 < *num2){
		return -1;
	} else if (*num1 > *num2) {
		return 1;
	} else {
		return 0;
	} 

}
int openReadSafe(const char* fileName){
	int fd;
	if (( fd == open(fileName, O_RDONLY)) == -1) {
		close_all();
		err(1, "Error while opening file %s", fileName);
	}

	return fd;
}

uint32_t readNumber(int fd, const char* fileName){
	uint32_t num;
	if(read(fd, &num, sizeof(num)) == -1) {
		close_all();
		err(1, "Error while reading %s", fileName);
	}

	return num;
}

void writeSafe(int fd, uint32_t num, const char* fileName){
	if(write(fd, &num, sizeof(uint32_t)) == -1){
		close_all();
		err(1, "Error while writing");
	}
}

void merge(const char* temp1, const char* temp2, int temp1Size, int temp2Size, const char* source_file) {
	fds[1] = openReadSafe(temp1);
	fds[2] = openReadSafe(temp2);

	if (( fds[0] = open(source_file, O_RDWR | O_TRUNC )) == -1) {
		err(1, "Error while reading from file %s", source_file);
	}

	uint32_t num1 = readNumber(fds[1], temp1);
	uint32_t num2 = readNumber(fds[2], temp2);
	
	int counter = 0, minSize = MIN(temp1Size, temp2Size);

	while(counter < minSize) {
		if(num1 < num2) {
			writeSafe(fds[0], num1, source_file);
			dprintf(1, "%d\n", num1);
			num1 = readNumber(fds[1], temp1);
		} else {
			writeSafe(fds[0], num2, source_file);
			dprintf(1, "%d\n", num2);
			num2 = readNumber(fds[2], temp2);
		}
		counter++;
	}

	while(counter < temp1Size) {
		writeSafe(fds[0], num1, source_file);
		dprintf(1, "%d\n", num1);
		num1 = readNumber(fds[1], temp1); 
		counter++;
	}

	while(counter < temp2Size) {
		writeSafe(fds[0], num2, source_file);
		dprintf(1, "%d\n", num2);
		num2 = readNumber(fds[2], temp2);
		counter++;
	}
}

void split(int halfCount, int fd, const char* temp_file, const char *source_file) {
	uint32_t num;
	uint32_t* half = malloc(halfCount * sizeof(uint32_t)); 
	if(half == NULL) {
		close(fd);
		errx(1, "Error while memory allocation");
	}
	int index = 0, bytes_count;
	while(index < halfCount && (bytes_count = read(fd, &num, sizeof(uint32_t))) == sizeof(uint32_t)) {
		half[index] = num;
		index++;
	}
	
	if(bytes_count == -1){
		close(fd);
		errx(1, "Error while reading from file %s", source_file);
	}

	qsort(half, halfCount, sizeof(uint32_t), source_file);
	int fd1;
	if ((fd1 = open(temp_file, O_RDWR | O_TRUNC | O_CREAT, S_IRWXU)) == -1) {
		free(half);
		err(1, "Error while opening file %s", source_file);
	}

	for (int i = 0; i < halfCount; ++i) {
		writeSafe(fd1, half[i], temp_file);
	}

	close(fd1);
	free(half);
}

void sortFile(const char* fileName){
	struct stat st;
	if(stat(fileName, &st) == -1){
		err(1, "Error stat file %s", fileName);
	}

	if((st.st_size % sizeof(uint32_t)) != 0){
		errx(1, "File %s does not only contain uint32_t numbers", fileName);
		exit(0);
	}

	ssize_t fileSize = st.st_size;
	if((fileSize > 1000000000)) {
		errx(1, "File %s contains too many numbers", fileName);
	}

	int fd = openReadSafe(fileName); 
	char temp1[11] = "tempXXXXXX", temp2[11] = "tempXXXXXX";
	if (mkstemp(temp1) == -1 || mkstemp(temp2) == -1) {
		err(1, "Could not mkstemp");
	}

	int numbersCount = fileSize / sizeof(uint32_t);
	int halfCount = numbersCount / 2;
	int otherHalfCount = numbersCount - halfCount;
	
	split(halfCount, fd, temp1, fileName);
	split(otherHalfCount, fd, temp2, fileName);

	close(fd);

	merge(temp1, temp2, halfCount, otherHalfCount, fileName);
	close_all();

	if (unlink(temp1) == -1 || unlink(temp2) == -1) {
		err(1, "Could not unlink");
	}

}

int main(int argc, char **argv){
	if(argc != 2) {
		errx(1, "Usage: %s <file_name>\n", argv[0]);
	}

	const char* fileName = argv[1];
	sortFile(fileName);
}

