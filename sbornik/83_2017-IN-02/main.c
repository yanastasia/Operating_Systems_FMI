#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

const int BUFF_SIZE = 4096;

void echo(void){
	char c;
	int b_read;
	while((b_read = read(0, &c, sizeof(c))) == sizeof(c)){
		if(write(1, &c, sizeof(c)) != sizeof(c)){
			err(1, "Failed to write to stdout");
		}
	}
	if(b_read == -1)
		err(1, "Failed to read from stdin");
}

void validate_argument(const char* arg){
	if(strlen(arg) > 4){
		errx(1, "Invalid %s length", arg);
	}
}

void execute(const char* command){
	char buf[BUFF_SIZE], c;
	int b_read, ind = 0;
	while(b_read = read(0, &c, sizeof(c)) == sizeof(c)){
		if(c == ' ' || c == '\n' || c == '\t'){
			buf[ind] = '\0';
			ind = 0;
			if(strlen(buf) > 0){
				validate_argument(buf);const pid_t child = fork();
				if(child == -1){
					err(1, "Couldn't fork");
				}

				if(child == 0){
					if(execlp(command, buf, buf, (char*)NULL) == -1){
						err(1, "Couldn't execute command %s %s.", command, buf);
					}
				}

				int status;
				if(wait(&status) == -1){
					err(1, "Couldn't wait for child process");
				}

				if(
	}

	if(b_read == -1){
		err(1, "Couldn't read from stdin");
	}

}

int main(int argc, char **argv){
	if(argc > 2){
		errx(1, "Invalid number of arguments");
	}

	const char *command = argv[1];
	if(argc == 2){
		validate_argument(command);
		execute(command);
	} else {
		echo();
	}
}

	
