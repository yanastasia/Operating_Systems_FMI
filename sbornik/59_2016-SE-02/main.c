#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct pair{
	uint32_t x;
	uint32_t y;
};

int main(int argc, char* argv[]){
	if (argc != 4)
		errx(1, "Inavlid number of arguments");

	int f1,f2;

	if ((f1 = open(argv[1], O_RDONLY)) == -1){
		err(2, "Can't open file");
	}

	if ((f2 = open(argv[2], O_RDONLY)) == -1){
		err(2, "Can't open file");
	}

	struct stat s;

	if (fstat(f1, &s) == -1){
		close(f1);
		close(f2);
		err(1, "Failed to get file status");
	}

	if (s.st_size%8 != 0){
		close(f1);
		close(f2);
		err(1, "File must be divisible by 8");
	}
	
	struct pair p;

	int bytes_count = 0;

	int f3;
	if ((f3 = open(argv[3], O_WRONLY | O_CREAT, S_IRWXU)) == -1){
		close(f1);
		close(f2);
		close(f3);
		err(1, "can't open file");
	}

	while((bytes_count = read(f1, &p, sizeof(p))) > 0){
		if(lseek(f2, p.x * 4, SEEK_SET) == -1){
			close(f1);
			close(f2);
			close(f3);
			err(1, "error in lseek");
		}

		uint32_t buf;

		for (int32_t i = 0; i < p.y; ++i){
			if (read(f2, &buf, sizeof(buf)) == -1){
				close(f1);
				close(f2);
				close(f3);
				err(1, "error in reading");
			}

			if(write(f3, &buf, sizeof(buf)) == -1){
				close(f1);
				close(f2);
				close(f3);
				err(1, "error in writing");
			}

		}
	}

	if(bytes_count == -1){
		close(f1);
		close(f2);
		close(f3);
		err(1, "error in reading");
	}

	close(f1);
	close(f2);
	close(f3);

	exit(0);
}

