#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdint.h>

const char L[3][4] = {"tic\n", "tac\n", "toe\n"};

int main(int argc, char** argv){
	if(argc!=3){
		errx(1, "Invalid number of argments");
	}
	
	char* endptr;
	long NC = strtol(argv[1], &endptr, 10);
	long WC = strtol(argv[2], &endptr, 10);

	if(NC < 1 || NC > 7){
		err(2, "Incorrect argv[1]");
	}

	if(WC < 1 || WC > 35){
		err(3, "Incorrect argv[2]");
	}

	uint8_t counter = 0;
	while(counter < WC){
		printf("\nParent\n");
		if(write(1, &L[counter%3], 4) != 4){
			err(1, "Error in writing");
		}

		counter++;

		for(int i = 0; i < NC && counter < WC; i++){
			const pid_t pid = fork();
			
			if(pid == -1){
				errx(6, "Fork failed");
			}

			if(pid == 0){
				printf("\nChild\n");
				if(write(1, &L[counter%3], 4) != 4){
					err(7, "Writing failed");
				}
				return 0;
			}
			int status;
			if(wait(&status) == -1){
				err(2, "Child process not finish");
			}
			if(!WIFEXITED(status) || WEXITSTATUS(status)!=0){
				err(1, "Child didn't finish");
			}


			counter++;
		}
	}
	return 0;
}
