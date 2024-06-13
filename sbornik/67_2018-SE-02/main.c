#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>

int cmp(const void* a, const void* b){
	return (*(uint32_t*)a - *(uint32_t*)b);
}

int main(int argc, char** argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	struct stat st;
	if(stat(argv[1], &st) == -1){
		err(1, "Stat failure");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(1, "Unable to open file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if(fd2 == -1){
		err(1, "Unable to open file %s", argv[2]);
	}

	uint32_t size = st.st_size / sizeof(uint32_t);

	if(size > 4194304){
		errx(1, "File too big");
	}

	uint32_t half = size / 2;
	uint32_t rhalf = size - half;

	uint32_t* arr=(uint32_t*)malloc(half * sizeof(uint32_t));

	int ft = open("temp.bin", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if(ft == -1)
		err(1, "Unable to open file temp.bin");
	int ft2 = open("temp2.bin", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if(ft2 == -1)
		err(1, "Unable to open file temp2.bin");

	int rs;
	if((rs=read(fd1, arr, sizeof(uint32_t) * half)) != half * sizeof(uint32_t)) {
		err(1, "Error while reading file %s", argv[1]);
	}

	qsort(arr, half, sizeof(uint32_t), cmp);

	int ws;
	if((ws=write(ft, arr, sizeof(uint32_t)*half)) != rs){
		err(1, "Error while writing in temp.bin");
	}

	free(arr);

	uint32_t* rarr=(uint32_t*)malloc(rhalf * sizeof(uint32_t));
	if((rs=read(fd1, rarr, rhalf * sizeof(uint32_t))) != rhalf* sizeof(uint32_t)) {
		err(1, "Error while reading in file %s", argv[1]);
	}
	qsort(rarr, rhalf, sizeof(uint32_t), cmp);

	if((ws=write(ft2, rarr, sizeof(uint32_t) * rhalf)) != rs)
		err(1, "Error while writing in temp2.bin");

	free(rarr);

	if(lseek(ft,0,SEEK_SET) == -1)
		err(1, "unable to seek temp.bin");
	if(lseek(ft, 0, SEEK_SET) == -1)
		err(1, "unable to seek temp2.bin");

	int rs2 = -1;
	uint32_t a;
	uint32_t b;
	while((rs=read(ft, &a, sizeof(a))) > 0 && (rs2=read(ft2, &b, sizeof(b))) > 0) {
		if(a<=b){
			if(write(fd2, &a, sizeof(a)) == -1)
				err(1, "Unable to write in %s", argv[2]);
			lseek(ft2, -1 * sizeof(b), SEEK_CUR);
		} else{
			if(write(fd2, &b, sizeof(b)) == -1)
				err(1, "Unable to write in %s", argv[2]);
			lseek(ft, -1*sizeof(a), SEEK_CUR);
		}
	}

	if(rs == sizeof(a)){
		if(write(fd2, &a, sizeof(a)) == -1)
			err(1, "Unable to write to file");
	}
	if(rs == -1 || rs == -1)
		err(1, "Reading failure");
	
	while((rs = read(ft, &a, sizeof(a))) == sizeof(a)) {
		if(write(fd2, &a, sizeof(a)) == -1)
			err(1, "unable to write to %s", argv[2]);
	}

	if(rs == -1)
		err(1, "unable to read from file");

	while((rs = read(ft2, &b, sizeof(b))) == sizeof(b)) {
		if(write(fd2, &b, sizeof(b)) == -1)
			err(1, "unable to write to %s", argv[2]);
	}
	if(rs2 == -1)
		err(1, "Unable to read from file");

	unlink("temp.bin");
	unlink("temp2.bin");
	close(fd1);
	close(fd2);
	close(ft);
	close(ft2);

	exit(0);
}

