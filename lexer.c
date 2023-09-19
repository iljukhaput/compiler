#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "dynamic_array.h"

enum { buf_size = 256 };

int lex(char *filename, int *ans)
{
	FILE *f;
	char c;
	int str_count = 0, char_count = 0;
	f = fopen(filename, "r");
	if (f == NULL) {
		perror(filename); // "open is failed");	
		exit(EXIT_FAILURE);
	}

	char buf[buf_size] = { 0 };
	struct arr_t *arr = arr_create(20);
	int count = 0;
	while ((c = fgetc(f)) != EOF) {
		if (c == ' ' || c == '\n' || c == '\t') {
			arr_add(&arr, buf, count);
			count = 0;
		} else {
			buf[count] = c;
			count++;
		}
	}
	arr_print(arr);

	int close_ok = fclose(f);
	if (close_ok == -1) {
		perror(filename);
		exit(EXIT_FAILURE);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int counts[2] = { 0 };

	if (argc != 2) {
		printf("incorrect command line arguments\n");
		return 1;
	}
	lex(argv[1], counts);
	printf("chars: %d\nlines: %d\n", counts[0], counts[1]);
	return 0;
}

