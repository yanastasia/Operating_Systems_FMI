#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//find "${1}" -type f ! -regex ".*\.hash$" -print0 | xargs -0 -I {} md5sum {}
int main(int argc, char* argv[]) {
	if(argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

    int pipe_fd[2];
    if(pipe(pipe_fd) == -1) {
        err(2, "Error: pipe failed");
    }

	pid_t pid = fork();
    if(pid == -1) {
        err(3, "Error: fork failed");
    }
 
    if(pid == 0) {
        close(pipe_fd[0]);
        if(dup2(pipe_fd[1], 1) == -1) {
            err(4, "Error: dup2 failed");
        }

        if(execlp("find", "find", argv[1], "-type", "f", "!", "-regex", ".*\.hash$", "-print0",(char*) NULL) == -1) {
            err(5, "Error: execlp failed");
        }
    }

    close(pipe_fd[1]);

    int pipe_fd2[2];
    if(pipe(pipe_fd2) == -1) {
        err(2, "Error: pipe failed");
    }

    pid_t pid_2 = fork();
    if(pid_2 == -1) {
      err(3, "Error: fork failed");
    }

    if(pid_2 == 0) {
        close(pipe_fd2[0]);
        if(dup2(pipe_fd[0], 0) == -1) {
            err(4, "Error: dup2 failed");
        }
        if(dup2(pipe_fd2[1], 1) == -1) {
            err(4, "Error: dup2 failed");
        }

        if(execlp("xargs", "xargs", "-0", "-I", "{}", "md5sum", "{}", (char*) NULL) == -1) {
            err(5, "Error: execlp failed");
        }
    }
	close(pipe_fd[0]);
	close(pipe_fd2[1]);
	
	char buff[300];
	int b_read;
	while((b_read = read(pipe_fd2[0], &buff, sizeof(buff))) > 0){

		//!!!!pochetak moj
		char* shasum = strtok(buff, " ");
		if(shasum == NULL){
			err(1, "Reading shasum");
		}

		char* file = strtok(NULL, "\n");
		if(file == NULL){
			err(1, "Reading fileName");
		}
		
		file = strchr(file, '/');
		if(file){
			file++;
		} else{
			errx(1, "Failed to strchr");
		}

		//sum preko echo | cut -d ' ' -f 1
		//file preko echo | rev | cut -d/ -f 1 | rev
		
		//pochetak na ivanu
		/*int ww_pipe1[2];
		if(pipe(ww_pipe1) == -1){
			err(1, "Pipe failed");
		}

		const pid_t ww_pid1 = fork();
		if(ww_pid1 == -1){
			err(1, "fork failed");
		}
		
		if(ww_pid1 == 0){
			close(ww_pipe1[0]);
			if(dup2(ww_pipe1[1], 1) == -1){
				err(1, "Dup2 failed");
			}
			
			if(execlp("echo", "echo", buff, (char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
		close(ww_pipe1[1]);

		int ww_pipe2[2];
		if(pipe(ww_pipe2) == -1){
			err(1, "pipe failed");
		}
		
		const pid_t ww_pid2 = fork();
		if(ww_pid2 == -1){
			err(1, "Fork failed");
		}

		if(ww_pid2 == 0){
			close(ww_pipe2[0]);
			if(dup2(ww_pipe1[0], 0) == -1){
				err(1, "dup2 failed");
			}
			if(dup2(ww_pipe2[1], 1) == -1){
				err(2, "dup2 failed");
			}
			
			if(execlp("cut", "cut", "-d", " ", "-f1", (char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
		
		close(ww_pipe1[0]);
		close(ww_pipe2[1]);
		
		char shasum[33];
		if(read(ww_pipe2[0], shasum, sizeof(shasum)) == -1){
			err(1, "failed reading from pipe");
		}
		
		int w_pipe1[2];
		if(pipe(w_pipe1) == -1){
			err(2, "Error pipe");
		}

		const pid_t w_pid1 = fork();
		if(w_pid1 == -1){
			err(4, "fork failed");
		}

		if (w_pid1 == 0){
			close(w_pipe1[0]);
			if(dup2(w_pipe1[1], 1) == -1){
				err(5, "Dup2 failed");
			}

			if(execlp("echo", "echo" , buff, (char*)NULL) == -1){
				err(1, "Execlp failed");
			}
		}

		close(w_pipe1[1]);

		int w_pipe2[2];
		if(pipe(w_pipe2) == -1){
			err(1, "pipe failed");
		}
		
		const pid_t w_pid2 = fork();

		if(w_pid2 == -1){
			err(3, "fork failed");
		}

		if(w_pid2 == 0){
			close(w_pipe2[0]);
			if(dup2(w_pipe1[0], 0) == -1){
				err(1, "dup2 failed");
			}
			if(dup2(w_pipe2[1], 1) == -1){
				err(1, "dup2 failed");
			}

			if(execlp("rev", "rev", (char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
		close(w_pipe1[0]);
		close(w_pipe2[1]);
		
		int w_pipe3[2];
		if(pipe(w_pipe3) == -1){
			err(1, "pipe failed");
		}
		
		const pid_t w_pid3 = fork();

		if(w_pid3 == -1){
			err(3, "fork failed");
		}

		if(w_pid3 == 0){
			close(w_pipe3[0]);
			if(dup2(w_pipe2[0], 0) == -1){
				err(1, "dup2 failed");
			}
			if(dup2(w_pipe3[1], 1) == -1){
				err(1, "dup2 failed");
			}

			if(execlp("cut", "cut", "-d/", "-f1", (char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
		close(w_pipe2[0]);
		close(w_pipe3[1]);
		
		int w_pipe4[2];
		if(pipe(w_pipe4) == -1){
			err(1, "pipe failed");
		}
		
		const pid_t w_pid4 = fork();

		if(w_pid4 == -1){
			err(3, "fork failed");
		}

		if(w_pid4 == 0){
			close(w_pipe4[0]);
			if(dup2(w_pipe3[0], 0) == -1){
				err(1, "dup2 failed");
			}
			if(dup2(w_pipe4[1], 1) == -1){
				err(1, "dup2 failed");
			}

			if(execlp("rev", "rev", (char*)NULL) == -1){
				err(1, "execlp failed");
			}
		}
		close(w_pipe3[0]);
		close(w_pipe4[1]);
		
		char file[256];
		if(read(w_pipe4[0], file, sizeof(file)) == -1){
			err(1, "failed reading from pipe");
		}
		//kraj
		*/

		const pid_t child_pid = fork();
		
		if(child_pid == -1){
			errx(1, "Fork failed");
		}

		if(child_pid == 0){
			strcat(file, ".hash");
			int hash_fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
			if(hash_fd == -1){
				errx(1, "Failed to open hashfile");
			}
			
			write(hash_fd, shasum, strlen(shasum));
			close(hash_fd);
			return 0;
		}

		wait(NULL);
	}
	
	if(b_read == -1){
		errx(3, "Failed reading");
	}

	return 0;
}
