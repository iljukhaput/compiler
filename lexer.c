#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "dynamic_array.h"

enum { buf_size = 256, start_size_arr = 20 };

int lex(char *filename, struct arr_t *arr)
{
	FILE *f;
	char c;
	int ok;
	f = fopen(filename, "r");
	if (f == NULL) {
		perror(filename); // "open is failed");	
		exit(EXIT_FAILURE);
	}

	char buf[buf_size] = { 0 };
	int count = 0;
	while ((c = fgetc(f)) != EOF) {
		if (c == ' ' || c == '\n' || c == '\t') {
			ok = arr_add(&arr, buf, count);
			if (ok != 0) {
				fprintf(stderr, "arr_add failed\n");
				return -1;
			}
			count = 0;
		} else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ';') {
			ok = arr_add(&arr, buf, count);
			if (ok != 0) {
				fprintf(stderr, "arr_add failed\n");
				return -1;
			}
			ok = arr_add(&arr, &c, 1);
			if (ok != 0) {
				fprintf(stderr, "arr_add failed\n");
				return -1;
			}
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
	if (argc != 2) {
		printf("incorrect command line arguments\n");
		return 1;
	}
	struct arr_t *arr = arr_create(start_size_arr);
	lex(argv[1], arr);
	return 0;
}

