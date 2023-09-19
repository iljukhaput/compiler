#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dynamic_array.h"

struct arr_t {
	char **arr;
	size_t size;
	size_t count;
};

struct arr_t *arr_create(size_t size)
{
	struct arr_t *arr = malloc(sizeof(struct arr_t));
	if (arr == NULL) {
		perror("arr_create: ");
		return NULL;
	}
	arr->arr = malloc(sizeof(int32_t) * size);
	if (arr->arr == NULL) {
		perror("arr_create: ");
		return NULL;
	}
	arr->size = size;
	arr->count = 0;
	return arr;
}

int arr_resize(struct arr_t **arr)
{
	struct arr_t *new_arr = arr_create((*arr)->size * 2);
	for (int i = 0; i < (*arr)->count; i++) {
		new_arr->arr[i] = (*arr)->arr[i];
	}
	new_arr->size = (*arr)->size * 2;
	if (new_arr->size < (*arr)->size) {
		fprintf(stderr, "size overflow\n"); 
		return -1; 
	}
	new_arr->count = (*arr)->count;
	for (int i = 0; i < (*arr)->count; i++) {
		free((*arr)->arr[i]);
	}
	free((*arr)->arr);
	free(*arr);
	*arr = new_arr;
	return 0;
}

int arr_add(struct arr_t **arr, char *str, size_t count)
{
	char *new_str = malloc(sizeof(char) * count);
	for (int i = 0; i < count; i++) {
		new_str[i] = str[i];
	}
	if ((*arr)->count == (*arr)->size) {
		int ok = arr_resize(arr);
		if (ok != 0) {
			return -1;
		}
	}
	(*arr)->arr[(*arr)->count] = new_str;
	(*arr)->count++;
	return 0;
}

void arr_print(struct arr_t *arr)
{
	for (int i = 0; i < arr->count; i++) {
		printf("%s\n", arr->arr[i]);
	}
}

/*
int main()
{
	struct arr_t *array = arr_create(3);
	printf("size: %zu\n", array->size);
	char str[256];
	scanf("%s", str);
	arr_add(&array, str);
	arr_add(&array, "123");
	arr_add(&array, "123");
	printf("size: %zu\n", array->size);
	arr_add(&array, "444 ");
	printf("size: %zu\n", array->size);
	printf("1) %s\n", array->arr[0]);
	printf("2) %s\n", array->arr[1]);
	printf("3) %s\n", array->arr[2]);
	printf("4) %s\n", array->arr[3]);
}
*/

