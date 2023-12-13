#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"
#include "hash_table.h"

struct hashmap_node_t {
	char *keyword;
	enum token_id token_id;
	struct hashmap_node_t *next;
};

struct hash_t {
	struct hashmap_node_t **htab;
	int len;
};

int htable_fill(struct hash_t *h);

struct hash_t *htable_create(int len)
{
	int fill_ok;
	struct hash_t *h = malloc(sizeof(struct hash_t));
	if (h == NULL) {
		return NULL;
	}
	h->htab = calloc(len, sizeof(struct hashmap_node_t *));
	if (h->htab == NULL) {
		return NULL;
	}
	h->len = len;

	fill_ok = htable_fill(h);
	if (fill_ok) {
		return h;
	}
	return NULL;
}

int get_hash_value(struct hash_t *h, char *keyword, size_t len_keyword)
{
	int i, hash_value = 0;
	for (i = 0; i < len_keyword; i++) {
		hash_value += keyword[i];
	}
	hash_value *= len_keyword;
	hash_value %= h->len;
	return hash_value;	
}

struct hashmap_node_t *htable_find(struct hash_t *h, char *keyword, size_t len_keyword)
{
	int hash_value;
	struct hashmap_node_t *tmp;

	hash_value = get_hash_value(h, keyword, len_keyword);
	tmp = h->htab[hash_value];
	while (tmp) {
		if ((strcmp(keyword, tmp->keyword)) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return 0;
}

int htable_insert(struct hash_t *h, char *keyword, size_t len_keyword, enum token_id token_id)
{
	int i, hash_value;
	struct hashmap_node_t *tmp, *new_node;

	if (htable_find(h, keyword, len_keyword)) {
		perror("Non-unique key");
		return 0;
	}

	new_node = malloc(sizeof(struct hashmap_node_t));
	if (new_node == NULL) {
		return 0;
	}
	new_node->keyword = malloc(sizeof(char) * len_keyword);
	for (i = 0; i < len_keyword; i++) {
		new_node->keyword[i] = keyword[i];
	}
	new_node->token_id = token_id;
	new_node->next = NULL;
	
	hash_value = get_hash_value(h, keyword, len_keyword);
	tmp = h->htab[hash_value];
	if (tmp == NULL) {
		h->htab[hash_value] = new_node;
	} else {
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = new_node;
	}
	return 1;
}

int htable_fill(struct hash_t *h)
{
	int ok = 0;
	ok = htable_insert(h, "{", 1, open_brace);
	ok = htable_insert(h, "}", 1, close_brace);
	ok = htable_insert(h, "(", 1, open_par);
	ok = htable_insert(h, ")", 1, close_par);
	ok = htable_insert(h, ";", 1, semicolon);
	ok = htable_insert(h, "int", 3, int_kw);
	ok = htable_insert(h, "return", 6, return_kw);
	return ok;
}

void htable_free(struct hash_t *h)
{
	int i;
	struct hashmap_node_t *tmp;
	for (i = 0; i < h->len; i++) {
		while (h->htab[i]) {
			tmp = h->htab[i];
			h->htab[i] = h->htab[i]->next;
			free(tmp);
		}
	}
	free(h);
}

enum token_id define_token_id(struct hash_t *h, char *keyword, size_t len_keyword)
{
	struct hashmap_node_t *node;
	node = htable_find(h, keyword, len_keyword);
	if (node) {
		return node->token_id;
	}
	return 0;
}

