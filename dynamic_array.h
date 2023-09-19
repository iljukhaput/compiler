#ifndef _DYNAMIC_ARRAY_
#define _DYNAMIC_ARRAY_

#include <stddef.h>

struct arr_t;
int arr_add(struct arr_t **arr, char *str, size_t count);
struct arr_t *arr_create(size_t size);
void arr_print(struct arr_t *arr);

#endif // _DYNAMIC_ARRAY_
