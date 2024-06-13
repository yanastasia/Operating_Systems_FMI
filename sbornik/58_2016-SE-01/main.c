#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void bubbleSort(unsigned char *arr, int size) {
    int i, j;
    for (i = 0; i < size-1; i++) {
        for (j = 0; j < size-i-1; j++) {
            if (arr[j] > arr[j+1]) {
            	unsigned char temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(int argc, char* argv[]) {
	if (argc != 2)
		errx(1, "Invalid number of arguments");


	int file = open(argv[1], O_RDWR);
	
	if (file == -1)
		err(1, "Error while opening file");
	
	off_t file_size = lseek(file, 0, SEEK_END);
	if (file_size == -1) {
        err(1, "Error while seeking file");
        close(file);
    }

    lseek(file, 0, SEEK_SET);

	unsigned char *buffer = malloc(file_size * sizeof(unsigned char));
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        close(file);
        exit (1);
    }

    size_t bytes_read = read(file, buffer, file_size);  
    if (bytes_read != file_size) {
        printf("Failed to read file\n");
        close(file);
        free(buffer);
        exit (1);
    }

    bubbleSort(buffer, bytes_read);

	if (close(file) == -1){
		err(1, "Unable to close file");
	}
	
	file = open(argv[1], O_WRONLY | O_TRUNC);
    if (file < 0) {
        printf("Failed to open file for writing" );
        free(buffer);
        exit (1);
    }

    size_t bytes_written = write(file, buffer, bytes_read);
    if (bytes_written != bytes_read) {
        printf("Failed to write to file\n");
        close(file);
        free(buffer);
        exit (1);
    }

    close(file);
    free(buffer);
	exit (0);
}

