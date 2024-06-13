#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>

void write_safe(const char *text){
	if(write(1, text, strlen(text)) != strlen(text)){
		err(1, "Failed to write to stdout");
	}
}

void validate_num(int num, int start, int end){
	if(num < start || num > end){
		err(1, "Number out of range");
	}
}

const char L[3][5] = { "tic ", "tac ", "toe\n"};

int main(int argc, char** argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	int NC = atoi(argv[1]);
	validate_num(NC, 1, 7);
	int WC = atoi(argv[2]);
	validate_num(WC, 1, 35);

	
	pid_t pid;
	int counter = 0;
	
	while(counter < WC){
		write_safe(L[counter%3]);
		counter++;

		for(uint8_t  i = 0; i < NC && counter < WC; ++i){

			if((pid = fork()) == -1){
				err(1, "Failed to fork");
			}

			if(pid == 0){
				write_safe(L[counter%3]);
				exit(0);
			}

			int status;

			if(wait(&status) == -1){
				err(1, "Couldn't wait for child process to finish");
			}

			if(!WIFEXITED(status)){
				err(1, "Child process not terminated normally");
			}

			if(WEXITSTATUS(status) != 0){
				err(1, "Exit status of child not equal to 0");
			}

			counter++;
		}
	}

	exit(0);
}


