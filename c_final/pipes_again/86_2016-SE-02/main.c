#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

void pipeCheck(int* fd){
	if(pipe(fd) == -1){
		err(1, "pipe failed");
	}
}

const pid_t forkCheck(){
	const pid_t pid = fork();
	if(pid == -1){
		err(2, "Fork failed");
	}
	return pid;
}

int main(void){
	char input[256];
	int b_read;
	while(1){
		if((b_read = read(0, &input, sizeof(input))) < 0){
			err(2, "Failed to read");
		}

		input[b_read-1] = '\0';

		if(strcmp(input, "exit") == 0){
			return 0;
		}

		const pid_t pid = forkCheck();
		if(pid == -1){
			err(1, "Failed to fork");
		}

		if(pid == 0){
			char command[256];
			strcpy(command, "/bin/");
			strcpy(command, input);

			if(execlp(command, input,(char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
	}
	
	return 0;
}
