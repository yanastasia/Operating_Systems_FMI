#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <sys/wait.h>

//a.init(1)		b.init(0)
//
//parent 		chilid
//a.wait	
//ding			b.wait
//b.signal		dong
//a.wait		a.signal
//sleep
//a.signal

void s(const int fd){
	if(write(fd, "o", 1) == -1){
		err(1, "Error in signal");
	}
}

void w(const int fd){
	char c;
	if(read(fd, &c, 1) == -1){
		err(5, "Error in wait");
	}
}

int main(int argc,char** argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	int N = atoi(argv[1]);
	int D = atoi(argv[2]);

	//pf1 - child	pf2 - parent
	int pf1[2];
	int pf2[2];

	if(pipe(pf1) == -1){
		err(1, "Pipe failed");
	}

	if(pipe(pf2) == -1){
		err(4, "Pipe failed");
	}

	const pid_t pid = fork();

	if(pid == -1){
		err(3, "Fork failed");
	}

	if(pid == 0){
		close(pf1[0]); //we close the child for reading
		close(pf2[1]); //we close the parent for writing

		for(int i = 0; i<N; i++){
			w(pf2[0]); //wait for the parent
			if(write(1, "DONG\n", 5) == -1){
				err(2, "Error in DONG");
			}
			s(pf1[1]);//child signals
		}
		return 0;
	}

	close(pf1[1]);
	close(pf2[0]);

	for(int j = 0; j<N; j++){
		if(write(1, "DING ", 5) == -1){
			err(6, "Error in DING");
		}

		s(pf2[1]);
		w(pf1[0]);

		if(sleep(D) != 0){
			err(9, "Error in sleep");
		}
	}
	return 0;
}
