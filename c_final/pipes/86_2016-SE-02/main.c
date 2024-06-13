#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>

int main(void){
	char command[100];
	int comBytes = 0;
	while(1){
		if((comBytes = read(0, &command, sizeof(command))) == -1){
			errx(3, "Failed reading");
		}
		command[comBytes-1] = '\0';
		
		if(strcmp(command, "exit") == 0){
			return 0;
		}

		const pid_t pid1 = fork();
		if(pid1 == -1){
			errx(1, "Fork failed");
		}

		if(pid1 == 0){
			char comm[100];
			strcpy(comm, "/bin/");
			strcat(comm, command);

			if(execlp(comm, command, (char*)NULL) == -1){
				errx(2, "Execlp failed");
			}

		}
	}
	return 0;
}

