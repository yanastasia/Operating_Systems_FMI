#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(int argc, char** argv){
	char com[4];
	if(argc == 2){
		if(strlen(argv[1]) > 4){
			errx(1, "command too long");
		}
		strcpy(com, argv[1]);
	}else{
		strcpy(com, "echo");
	}

	printf(com);
	return 0;
}
