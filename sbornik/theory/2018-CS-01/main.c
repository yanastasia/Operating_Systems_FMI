#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv){
	//char* buff = "Hello world\n";
	//char* buff2 ="Hello world!\n";

	//char* msg = "fork()\n";
	//char* msg2 = "after fork()\n\n";
	int p;
	//dprintf(1, "p: %d\n", p);
	if (fork()==0){ 
		printf("Hello world from p: %d\n", p);
		//write(1,buff,strlen(buff));
		//write(1,msg,strlen(msg));
	}
	p=fork();
	printf("Hello world from p: %d\n", p);
	//write(1,buff2,strlen(buff2));
	//dprintf(1, "p2: %d\n", p);
	//write(1, msg2, strlen(msg2));

	//write(1, argv[0], sizeof(argv[0]));
}
