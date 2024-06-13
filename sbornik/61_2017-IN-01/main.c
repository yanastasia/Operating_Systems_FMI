#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>

struct pos{
	uint16_t position;
	uint8_t length;
	uint8_t _;
};

int main(int argc, char **argv){
	if (argc != 5){
		errx(1, "Invalid number of arguments");
	}

	int dat_fd, idx_fd;

	if (( dat_fd = open(argv[1], O_RDONLY) ) == -1){
		err(1, "Error while opening %s", argv[1]);
	}

	if (( idx_fd = open(argv[2], O_RDONLY) ) == -1){
        err(1, "Error while opening %s", argv[2]);
    }

    struct stat s;
    if(fstat(idx_fd, &s) == -1){
    	err(1, "Error in stat");
	}

	if(s.st_size %4 != 0){
		errx(1, "Error with %s format", argv[2]);
	}

	int dat_size = s.st_size;

	int bytes_count = 0;
	struct pos p;
	
	int dat_out, idx_out;

	if ((dat_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
		err(1, "Error opening %s", argv[3]);
	}

	if ((idx_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) ==     -1){
		err(1, "Error opening %s", argv[4]);
	}

	uint16_t offset = 0;

	while(( bytes_count = read(idx_fd, &p, sizeof(p))) > 0){
		if(p.position + p.length > dat_size) {
			errx(1, "Inconsistent");
		}

		uint8_t buf;
		lseek(dat_fd, p.position, SEEK_SET);
		if(read(dat_fd, &buf, sizeof(buf)) == -1){
			err(1, "Reading from dat");
		}

		if(buf < 0x41 || buf > 0x5A) {
			continue;
		}

		if(write(dat_out, &buf, sizeof(buf)) == -1){
			err(1, "Error with writing to dat");
		}

		struct pos out_pos;
		out_pos.position = offset;
		out_pos.length = p.length;
		out_pos._ = 0;

		if(write(idx_out, &out_pos, sizeof(out_pos)) == -1) {
			err(1, "Error with writing to idx");
		}

		for(int i = 0; i < p.length - 1; ++i) {
			if(read(dat_fd, &buf, sizeof(buf)) == -1){
				err(1, "Reading from dat");
			}

			if(write(dat_out, &buf, sizeof(buf)) == -1){
				err(1, "Writing to dat");
			}

		}

		offset += p.length;
	}

	if(bytes_count == -1) {
		err(1, "Error with reading");
	}

	close(idx_fd);
	close(dat_fd);
	close(dat_out);
	close(idx_out);

	return 0;
}

