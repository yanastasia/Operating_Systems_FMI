#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>

void write_safe(const char* text){
	if(write(1, text, strlen(text))!=strlen(text)){
		err(1, "Couldn't write to stdout");
	}
}

void validate_number(int number, int start, int end, const char* num_type){
	if(number < start || number > end){
		errx(1, "Invalid %s. Number should be between %d-%d", num_type, start, end);
	}
}

const char words[3][5] = {"tic ", "tac ", "toe\n"};

int main(int argc, char** argv){
	if(argc!=3){
		errx(1, "Invalid number of arguments");
	}

	char* endptr;
	int NC, WC;
	NC = atoi(argv[1]);
	validate_number(NC, 1, 7, "NC");
	WC = atoi(argv[2]);
	validate_number(WC, 1, 35, "WC");
	uint8_t counter = 0;
	
	while(counter < WC){
		write_safe(words[counter % 3]);
		counter++;

		for(uint8_t i = 0; i < NC && counter < WC; i++){
			const pid_t child = fork();
			if(child == -1){
				err(1, "Fork failed");
			}

			if(child == 0){
				write_safe(words[counter%3]);
				exit(0);
			}

			int status;
			if(wait(&status) == -1){
				err(1, "Couldn't wait for child process to finish");
			}

			if(!WIFEXITED(status)){
				err(1, "Child process didn't terminate normally");
			}

			if(WEXITSTATUS(status) != 0){
				err(1, "Exit status of child isn't 0");
			}

			counter++;
		}
	}
}
