#ifndef _DYNAMIC_ARRAY_
#define _DYNAMIC_ARRAY_

#include <stddef.h>

// "word" used in automat.c, "entry" - in parser.c
enum token_id { open_brace=1, close_brace, open_par, close_par, semicolon, int_kw,
				return_kw, identifier, int_literal, punct_mark, word, entry }; 
struct token_t;
struct arr_t;

int arr_add(struct arr_t **arr, char *str, size_t count, enum token_id);
struct arr_t *arr_create(size_t size);
enum token_id token_id(struct token_t *token);
struct token_t *get_token(struct arr_t *arr, int num);

void arr_print(struct arr_t *arr);

#endif // _DYNAMIC_ARRAY_
