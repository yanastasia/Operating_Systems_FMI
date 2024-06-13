#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char ** argv){
	char c;
	int rs;

	if(argc == 3 && strcmp(argv[1], "-c") == 0){
		if(strlen(argv[2]) == 1){
			int pos = argv[2][0] - '0';
			int counter = 1;
			while((rs = read(0, &c, sizeof(c)))){
				if(counter == pos){
					if(write(1, &c, sizeof(c)) != sizeof(c)){
						err(1, "Error to stdout");
					}
				}
				++counter;
			}
				if( rs == -1){
					err(1, "Error from stdim");
				}
		} else if (strlen(argv[2]) == 3) {
			int from = argv[2][0] - '0';
			int to = argv[2][2] - '0';
			if(from>to){
				errx(1, "wrong arguments");
			}

			int counter = 1;
			while ((rs = read(0, &c, sizeof(c)))) {
				if(counter >= from && counter <= to) {
					if(write(1, &c, sizeof(c)) != sizeof(c)){
						err(1, "Error to stdout");
					}
				}
				++counter;
			}
			if(rs == -1){
				err(1, "Unable from stdin");
			}

		}
	} else if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0 && argc == 5){
		if(strlen(argv[4]) == 1) {
			int counter = 1;
			int pos = argv[4][0] - '0';
			int delimeter = argv[2][0];

			while((rs = read(0, &c, sizeof(c)))){
				if(delimeter == c){
					counter++;
					continue;
				}

				if(pos == counter){
					if(write(1, &c, sizeof(c)) != sizeof(c)){
						err(1, "Error to stdout");
					}
				}
			}
			if(rs == -1){
				err(1, "Unable from stdin");
			}
		} else if (strlen(argv[4]) == 3){
			int from = argv[4][0] - '0';
			int to = argv[4][2] - '0';
			int counter;
			char delimeter = argv[2][0];

			while((rs = read(0, &c, sizeof(c)))){
				if(delimeter == c) {
					counter++;
					continue;
				}

				if(counter>=from && counter<=to){
					if(write(1, &c, sizeof(c)) != sizeof(c)){
						err(1, "Error to stdout");
					}
				}
			}
		}
	}
}

