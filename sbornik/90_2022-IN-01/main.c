#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void write_safe(const char* str){
	if(write(1, str, strlen(str)) != strlen(str)){
		err(1, "Couldn't write to stdout");
	}
}

void check_args(const char* argv1, const char* argv2){
	if(strlen(argv1) != 1 || strlen(argv2) != 1) {
		errx(1, "Invalid arguments");
	}

	char num1 = argv1[0], num2 = argv2[0];

	if(!(num1 <= '9' && num1 >= '0') || !(num2 <= '9' && num2 >= '0')){
		errx(1, "Invalid arguments");
	}
}

int main(int argc, char** argv){
	if(argc != 3){
		errx(1, "Invalid arguments");
	}

	check_args(argv[1], argv[2]);

	int N = (int)argv[1][0] - '0', D = (int)argv[2][0] - '0';

	for(int i = 0; i < N; ++i){
		write_safe("DING\n");

		const pid_t child = fork();
		if(child == -1){
			err(1, "Fork failed");
		}

		if(child == 0){
			write_safe("DONG\n");
			exit(0);
		}

		int status;
		if(wait(&status) == -1){
			err(1, "Couldn't wait for child process");
		}

		if(!WIFEXITED(status)){
			err(1, "Child process not terminated normally");
		}

		if(WEXITSTATUS(status) != 0){
			err(1, "Exit status of child process != 1");
		}

		sleep(D);
	}
	exit(0);
}


