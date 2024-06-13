#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>

int main(int argc, char** argv){
	if (argc < 3){
		errx(1, "Wrong number of arguments");
	}

	int fd=open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	int time_limit = argv[1][0] - '0';
	int second_time = 0;
	while(1){
		int pfd[2];
		if(pipe(pfd) == -1){
			errx(2, "Pipe failed");
		}
		
		int finish;
		int exit_status;
		const pid_t pid = fork();
		if(pid == -1){
			errx(1, "Fork failed");
		}

		if(pid==0){
			int start = time(NULL);
			close(pfd[0]);
			if(write(pfd[1], &start, sizeof(start)) != sizeof(start)){
					errx(3, "Write failed");
			}
			if(execvp(argv[2], argv+2) == -1){
				errx(4, "Execvp failed");
			}
		}
		close(pfd[1]);
		int status;
		wait(&status);
		finish=time(NULL);
		exit_status=WEXITSTATUS(status);

		int start;
		if(read(pfd[0], &start, sizeof(start)) != sizeof(start)){
			errx(5, "Reading failed");
		}
		
		if(dprintf(fd, "%d %d %d\n", start, finish, exit_status) == -1){
			errx(1,"Failed dprintf");
		}
		
		if(exit_status != 0 && finish - start < time_limit){
			if(second_time){
				break;
			} else{
				second_time = 1;
			}
		} else{
			second_time = 0;
		}

		//close(pfd[0]);
	}
	close(fd);	
	return 0;
}
