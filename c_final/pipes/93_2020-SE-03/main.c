#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct {
	char fname[8];
	uint32_t offset;
	uint32_t length;
}trinity;

uint16_t xor_elements(const uint16_t* elements, uint32_t length){
	uint16_t result = elements[0];
	for(uint32_t i = 1; i<length; i++){
			result^=elements[i];
	}

	return result;
}

int main(int argc, char** argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments");
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		err(2, "Failed to open file");
	}

	struct stat st;
	if(stat(argv[1], &st) == -1){
		err(3, "Failed to stat");
	}

	if((st.st_size%sizeof(trinity)) != 0){
		err(4, "File corrupted");
	}

	int n = st.st_size/sizeof(trinity);
	uint16_t result = 0;

	if(n>8){
		err(1, "File too big");
	}

	for(int i = 0; i<n; i++){
		int pf[2];

		if(pipe(pf) == -1){
			err(5, "Failed to pipe");
		}

		char fileName[8];
	
		if(read(fd, fileName, 8*sizeof(char)) != 8*sizeof(char)){
			errx(1, "Failed reading");
		}

		int fd1 = open(fileName, O_RDONLY);
		if(fd1 == -1){
			errx(1, "Failed with open");
		}

		uint32_t offset;
		if(read(fd, &offset, sizeof(uint32_t))!=sizeof(uint32_t)){
			errx(1, "Error while reading");
		}
		
		if(lseek(fd1, offset, SEEK_SET) == -1){
			errx(3, "Failed seeking");
		}

		uint32_t length;
		if(read(fd, &length, sizeof(uint32_t))!=sizeof(uint32_t)){
			errx(1, "Error while reading");
		}

		uint16_t elements[length];

		for(uint32_t j = 0; j< length; j++){
			if(read(fd1, &elements[j], sizeof(uint16_t)) != sizeof(uint16_t)){
				errx(1, "Error while reading element");
			}
		}

		const pid_t pid = fork();
		if(pid == -1){
			err(6, "Failed to fork");
		}
		if(pid == 0){
			close(pf[0]);
			uint16_t c_result = xor_elements(elements, length);
			write(pf[1], &c_result, sizeof(uint16_t));
			close(pf[1]);
		}else{
			close(pf[1]);

			int status;
			wait(&status);
			if(!WIFEXITED(status)){
				errx(2, "Child process problem");
			}
			
			uint16_t child_result;
			read(pf[0], &child_result, sizeof(uint16_t));

			result^=child_result;

			close(pf[0]);
		}
	}
	close(fd);
	
	printf("result: %04X\n", result);
	return 0;
}
