#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

void write_safe(const char* what){
	if(write(1, what, strlen(what)) != strlen(what)){
		err(1, "Couldn't write to stdout");
	}
}

void val_num(int num, int start, int end){
	if(num < start || num > end){
		err(1, "Number out of range");
	}
}

const char L[3][5] = {"tic ", "tac ", "toe\n"};

int main(int argc, char **argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}
	
	int NC = atoi(argv[1]);
	val_num(NC, 1, 7);
	int WC = atoi(argv[2]);
	val_num(WC, 1, 35);

	int counter=0;
	while(counter < WC){
		write_safe(L[counter%3]);
		counter++;

		for(int i = 0; i < NC && counter < WC; ++i){
			const pid_t pid = fork();
	        if(pid == -1){
	             err(1, "Fork failed");
 		    }

			if(pid == 0){
				write_safe(L[counter%3]);
				exit(0);
			}

			int status;
			if(wait(&status) == -1){
				err(1, "Didn;'t wait");
	}

			if(!WIFEXITED(status)){
				err(1, "Not normal exit");
			}

			if(WEXITSTATUS(status) != 0){
				err(1, "exit not 0");
			}

			counter++;
		}
	}
	exit(0);
}


