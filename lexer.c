#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "dynamic_array.h"

enum { buf_size = 256, start_size_arr = 20 };

enum token_id defining_token(const char *str)
{
	if (strcmp(str, "int") == 0) {
		return int_kw;
	} else if (strcmp(str, "return") == 0) {
		return return_kw;
	} else if (*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z' 
			   || *str == '_') {
		return identifier;
	} else if (*str >= '0' && *str <= '9') {
		return int_literal;
	} else {
		return -1;
	}
}

int space_char(struct arr_t *arr, char *buf, size_t *count)
{
	if (*count == 0) {
		return 0;
	}
	enum token_id id = defining_token(buf);
	int ok = arr_add(&arr, buf, *count, id);
	if (ok != 0) {
		fprintf(stderr, "arr_add failed\n");
		return -1;
	}
	*count = 0;
	return 0;
}

int punctuation_mark(struct arr_t *arr, char *buf, size_t *count, char c, enum token_id id)
{
	int ok;
	if (*count != 0) {
		enum token_id buf_token_id = defining_token(buf);
		ok = arr_add(&arr, buf, *count, buf_token_id);
		if (ok != 0) {
			fprintf(stderr, "arr_add failed\n");
			return -1;
		}
	}
	ok = arr_add(&arr, &c, 1, id);
	if (ok != 0) {
		fprintf(stderr, "arr_add failed\n");
		return -1;
	}
	*count = 0;
	return 0;
}

void analyze_char(struct arr_t *arr, char *buf, size_t *count, char c)
{
	switch (c) {
	case ' ':
		;
	case '\n':
		;
	case '\t':
		;
		space_char(arr, buf, count);
		break;
	case '{':
		punctuation_mark(arr, buf, count, c, open_brace);
		break;
	case '}':
		punctuation_mark(arr, buf, count, c, close_brace);
		break;
	case '(':
		punctuation_mark(arr, buf, count, c, open_par);
		break;
	case ')':
		punctuation_mark(arr, buf, count, c, close_par);
		break;
	case ';':
		punctuation_mark(arr, buf, count, c, semicolon);
		break;
	default:
		buf[*count] = c;
		(*count)++;
	}

}

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
	size_t count = 0;
	while ((c = fgetc(f)) != EOF) {
		analyze_char(arr, buf, &count, c);
		/*
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
		*/
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

