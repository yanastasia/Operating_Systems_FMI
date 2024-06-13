#include <string.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool BUFF[256];
char SUB[256];

void mark_visited_chars(const char* set){
	int set_length = strlen(set);
	for(int i = 0; i < set_length; ++i){
		BUFF[(int)set[i]] = 1;
	}
}

void write_safe(char c){
	if((write(1, &c, sizeof(c)))!=sizeof(c)){
		err(1, "Couldn't write to STDOUT");
	}
}

void delete_chars(const char* str, const char* set, int str_length) {
	mark_visited_chars(set);
	for(int i = 0; i < str_length; ++i){
		if ( BUFF[(int)str[i]] == 0){
			write_safe(str[i]);
		}
	}
}

void sub_chars(const char* str, const char* set, int str_length){
	mark_visited_chars(set);
	bool WRITTEN[256];
	for(int i = 0; i < str_length; ++i){
		if(BUFF[(int)str[i]] == 0){
			write_safe(str[i]);
		} else if (BUFF[(int)str[i]] == 1 && WRITTEN[(int)str[i]] == 0) {
			WRITTEN[(int)str[i]] = 1;
			write_safe(str[i]);
		}
	}
}

void mark_sub_chars(const char* set1, const char* set2){
	int set_length = strlen(set1);
	for(int i = 0; i < set_length; ++i){
		SUB[(int)set1[i]] = set2[i];
	}
}

void sub_sets(const char* str, const char* set1, const char* set2, int str_length){
	mark_visited_chars(set1);
	mark_sub_chars(set1, set2);

	for(int i = 0; i < str_length; ++i){
		if(BUFF[(int)str[i]] == 0){
			write_safe(str[i]);
		} else if (BUFF[(int)str[i]] == 1) {
			write_safe(SUB[(int)str[i]]);
		}
	}

}

int main(int argc, char** argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments");
	}

	char str[4096];

	if(read(0, &str, sizeof(str)) == -1){
		err(1, "Couldn't read from STDIN");
	}

	int str_length = strlen(str);
	char set1[4096], set2[4096];

	if(strcmp(argv[1], "-d") == 0){
		strcpy(set1, argv[2]);
		delete_chars(str, set1, str_length);
	} else if (strcmp(argv[1], "-s") == 0){
		strcpy(set1, argv[2]);
		sub_chars(str, set1, str_length);
	} else {
		if(strlen(argv[1]) != strlen(argv[2])){
			errx(1, "Length of SET1 is not equal to length of SET2");
		}

		strcpy(set1, argv[1]);
		strcpy(set2, argv[2]);
		sub_sets(str, set1, set2, str_length);
	}

	exit(0);
}

