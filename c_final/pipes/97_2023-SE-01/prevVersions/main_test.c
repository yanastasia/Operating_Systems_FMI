#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char shasum[33];
	char fileName[256];
} md;

int main(int argc, char** argv){
	int fd = open(argv[1], O_RDONLY);

	char buff[300];
	//char sum[33];
	//char file[256];
	md data[100];
	int count = 0;
	int b_read;
	while((b_read = read(fd, buff, sizeof(buff))) > 0 && count < 100){
		char* sum = buff;
		if(sum == NULL){
			err(1, "Error while reading shasum");
		}

		char* file= strchr(buff, ' ');
		*file = '\0';
		file++;
		if(file == NULL){
			err(1, "Error while reading fileName");
		}
		
		strcpy(data[count].shasum, sum);
		strcpy(data[count].fileName, file);
		count++;
	}

	if(b_read == -1){
		errx(3, "Failed reading");
	}

	for(int i = 0; i<count; i++){
		printf("Data %d:\n", i+1);
		printf("shasum: %s\n", data[i].shasum);
		printf("fileName: %s\n", data[i].fileName);
		printf("\n");
	}

	close(fd);
	return 0;
}
