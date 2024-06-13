#include <sys/types.h>
 #include <unistd.h>
 #include <stdlib.h>
 int  main(void)
 {
 pid_t p1,p2,p3;
 write(1,"A",1);
 p1 = fork();
 if (p1 == 0) {
	write(2,"BB",2);
 	p2 = fork();
 	if (p2 > 0){
 		write(1,"C",1);
 		p3 = fork();
 		write(2,"DD",2);
 		if (p3 > 0) exit(0);
 	}
 }
 	write(1,"A",1);
 	write(1,"\n",sizeof("\n"));
 	exit(0);
 }
