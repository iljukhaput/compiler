#ifndef _DYNAMIC_ARRAY_
#define _DYNAMIC_ARRAY_

#include <stddef.h>

enum token_id { open_brace, close_brace, open_par, close_par, semicolon, int_kw,
				return_kw, identifier, int_literal, space };
struct arr_t;

int arr_add(struct arr_t **arr, char *str, size_t count, enum token_id);
struct arr_t *arr_create(size_t size);
void arr_print(struct arr_t *arr);

#endif // _DYNAMIC_ARRAY_
