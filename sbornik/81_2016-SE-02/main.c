#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

const int BUFF_SIZE = 4096;

int main(void){
	int b_read;
	char buf[BUFF_SIZE];
	const char *cmd = "";
	if(write(1, cmd, strlen(cmd)) != (int)strlen(cmd)){
		err(1, "Failed writing");
	}

	while((b_read = read(0, &buf, sizeof(buf))) > 0){
		buf[b_read - 1] = '\0';
		const pid_t child = fork();
		if(child == -1){
			err(1, "Failed fork");
		}

		if(strcmp(buf, "exit") == 0){
			exit(0);
		}

		if(child == 0){
			char command[BUFF_SIZE];
			strcpy(command, "/bin/");
			strcat(command, buf);
			if(execl(command, buf, (char*)NULL) == -1){
				err(1, "Could not exec %s", buf);
			}
		}

		int status;
		if(wait(&status) == -1){
			err(1, "Could not wait child");
		}

		if(!WIFEXITED(status)) {
			errx(1, "Child process not terminated normally");
		}

		if(WEXITSTATUS(status) != 0){
			errx(1, "Child process finished with exit code not 0");
		}

		if(write(1, cmd, strlen(cmd)) != strlen(cmd)){
			err(1, "Failed writing");
		}

	}

	if(b_read == -1){
		err(1, "Failed reading");
	}
}

