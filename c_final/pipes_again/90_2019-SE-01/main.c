#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>

int main(int argc, char** argv){
	if(argc < 3){
		err(1, "Invalid number of arguments");
	}

	int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	int time_limit = argv[1][0] - '0';
	int second_time = 0;
	while(1){
		int pfd[2];
		if(pipe(pfd) == -1){
			err(1, "pipe failed");
		}
		
		int exit_status;
		const pid_t pid = fork();
		if(pid == -1){
			err(1, "Fork failed");
		}
		if(pid == 0){
			int start = time(NULL);
			close(pfd[0]);
			if(write(pfd[1], &start, sizeof(start))!=sizeof(start)){
				err(3, "failed to write");
			}
			if(execvp(argv[2], argv+2) == -1){
				err(4, "failed at execvp");
			}
		}
		close(pfd[1]);
		int status;
		wait(&status);
		int finish = time(NULL);
		exit_status = WEXITSTATUS(status);

		int start;
		if(read(pfd[0], &start, sizeof(start)) != sizeof(start)){
			err(1, "failed at reading from pipe");
		}

		if(dprintf(fd, "%d %d %d\n", start, finish, exit_status) == -1){
			err(1, "failed dprontf");
		}

		if(exit_status != 0 && finish - start < time_limit){
			if(second_time){
				break;
			}else{
				second_time = 1;
			}
		} else{
			second_time = 0;
		}

		close(pfd[0]);
	}

	close(fd);
	return 0;

}
