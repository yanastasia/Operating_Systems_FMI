#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char **argv){
	if(argc < 3){
		err(1, "Invalid number of arguments");
	}

	int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	int time_limit = argv[1][0] - '0';
	int second_time = 0;

	while(1){
		int pf[2];
		if(pipe(pf) == -1){
			err(1, "Failed to pipe");
		}

		int finish;
		int exit_status;
		pid_t pid = fork();
		if(pid == -1){
			err(1, "Failed to fork");
		}

		if(pid == 0){
			int start = time(NULL);
			close(pf[0]);
			if(write(pf[1], &start, sizeof(start)) != sizeof(start))
				err(1, "Error while writing");
			if(execvp(argv[2], argv + 2) == -1){
				err(1, "Error while executing");
			}
		}
		close(pf[1]);
		int status;
		if(wait(&status) == -1){
			err(1, "Couldn't wait for child to finish");
		}

		if(!(IWIFEXITED(status))){
			err(1, "Child process didn't terminate normally");
		}

		if(WEXITSTATUS(status) != 0){
			err(1, "Child process exit status not 0");
		}

		int start;
		if(read(pf[0], &start, sizeof(start)) != sizeof(start)){
			err(1, "Error while reading");
		}

		dprintf(1, "%d %d %d\n", start, finish, WEXITSTATUS(status));

		if(WEXITSTATUS(status) != 0 && finish - start < time_limit) {
			if(second_time)
				exit(0);
			else
				second_time = 1;
		} else 
			second_time = 0;
		close(pf[0]);
	}
}


