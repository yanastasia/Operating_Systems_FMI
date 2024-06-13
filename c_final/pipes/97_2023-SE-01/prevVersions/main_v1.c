#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

void generate_hash(const char* file_path){
	pid_t pid=fork();

	if(pid==-1){
		errx(1, "Fork failed");
	} else if(pid==0){
		char hash_file[256];
		strcpy(hash_file, file_path);
		strcat(hash_file, ".hash");
		
		int fd = open(hash_file, O_WRONLY | OCREAT | O_TRUNC, 0666);
		if(fd == -1){
			errx(3, "Failed to open for writing");
		}

		dup2(fd, 1);

		if(execlp("md5sum", "md5sum", file_path, (char*)NULL) == -1){
			errx(2, "Execlp failed");
		}
		
		close(fd);
		return 0;
	}
}

int main(int argc, char** argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}

	char find_command[512];
	//traversal through directories

