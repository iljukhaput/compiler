#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include "dynamic_array.h"

struct hash_t;
struct hashmap_node_t;

struct hash_t *htable_create(int len);
enum token_id define_token_id(struct hash_t *h, char *keyword, size_t len_keyword);
void htable_free(struct hash_t *h);

#endif // _HASH_TABLE_
