#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>

void read_fd(int fd, uint8_t num_lines, uint8_t *l_count) {
	uint8_t sym;
	int bytes_count = -1;

	if (num_lines) {
    	dprintf(1, "%d: ", (*l_count));
    	(*l_count)++;
	}

	bytes_count = read(fd, &sym, sizeof(sym));
	
	while (true) {
		if ( (write(1, &sym, sizeof(sym))) == -1) {
			err(1, "Error while writing");
		}		
		if (num_lines && sym == '\n' && (bytes_count = read(fd, &sym, sizeof(sym)) > 0)){  
            dprintf(1, "%d: ", (*l_count));
            (*l_count)++;
            continue;
        }

		if(!(bytes_count = read(fd, &sym, sizeof(sym)))){
			break;
		}
	}

	if (bytes_count == -1) {
		err(1, "Error while reading");
	}
}

int main(int argc, char **argv){
	if(argc == 1){
		read_fd(0, 0, 0);
	} else if (argc > 1) {
		uint8_t num_lines = 0;
		if(strcmp(argv[1], "-n") == 0) {
			num_lines = 1;
			if (argc == 2) {
				read_fd(0, num_lines, 0);
			}
		}
		uint8_t lines_count = num_lines;
		for (int i = 1+ num_lines; i < argc; i++) {
			int fd;
			if (strcmp(argv[i], "-") != 0) {
				fd = open(argv[i], O_RDONLY);
				if(fd == -1){
					err(1, "Error while opening file");
				}
			} else {
				fd = 0;
			}
			read_fd(fd, num_lines, &lines_count);
		}
	}
}
