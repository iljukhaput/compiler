#include <stdio.h>
#include <stdlib.h>
#include "automat.h"
#include "dynamic_array.h"

// #define DEGUB_

enum node_type { program, function, statement, exp };
char *node_type[] = { "program", "function", "statement", "expression" };

struct ast_node {
	enum node_type type;
	struct ast_node *child;
};

struct ast_node *parse_exp(struct arr_t *arr, int *token_num)
{
	struct ast_node *new_node;
	new_node = malloc(sizeof(struct ast_node));
	if (new_node == NULL) {
		return NULL;
	}
	if (token_id(get_token(arr, *token_num)) != int_literal) {
		return NULL;
#ifdef DEBUG_
		printf("error in parse_exp");
#endif
	}
	(*token_num)++;
	new_node->type = exp;
	new_node->child = NULL;
	return new_node;

}

struct ast_node *parse_statement(struct arr_t *arr, int *token_num)
{
	int i;
	enum token_id rule[] = { return_kw, entry, semicolon };
	struct ast_node *new_node;
	new_node = malloc(sizeof(struct ast_node));
	if (new_node == NULL) {
		return NULL;
	}
	
	new_node->type = statement;
	for (i = 0; i < sizeof(rule)/sizeof(enum token_id); i++) {
		if (token_id(get_token(arr, *token_num)) == rule[i]) {
			(*token_num)++;
			continue;
		} else if (rule[i] == entry) {
			new_node->child = parse_exp(arr, token_num);
			if (new_node->child == NULL) {
				return NULL;
			}
		} else {
#ifdef DEBUG_
			printf("error in parse_statement");
#endif
			return NULL;
		}
	}
	return new_node;
}


struct ast_node *parse_function (struct arr_t *arr)
{
	int i;
	struct ast_node *new_node;
	enum token_id rule[] = { int_kw, identifier, open_par, close_par, 
		open_brace, entry, close_brace };

	new_node = malloc(sizeof(struct ast_node));
	if (new_node == NULL) {
		return NULL;
	}

	new_node->type = function;
	for (i = 0; i < sizeof(rule)/sizeof(enum token_id); i++) {
		if (token_id(get_token(arr, i)) == rule[i]) {
			continue;
		} else if (rule[i] == entry) {
			new_node->child = parse_statement(arr, &i); // 
			if (new_node->child == NULL) {
				return NULL;
			}
		} else {
#ifdef DEBUG_
			printf("error in parse_function");
#endif
			return NULL;
		}
	}
	return new_node;
}

struct ast_node *parse_program(char *filename)
{
	struct arr_t *token_arr;
	struct ast_node *new_node;
	new_node = malloc(sizeof(struct ast_node));
	if (new_node == NULL) {
		return NULL;
	}
	
	new_node->type = program;
	token_arr = get_token_arr(filename);
	new_node->child = parse_function(token_arr);
	if (new_node->child == NULL) {
		return NULL;
	}
	return new_node;
}

void print_tree(struct ast_node *root)
{
	while (root != NULL) {
		printf("%s\n", node_type[root->type]);
		root = root->child;
	}
}

/*
int main(int argc, char **argv)
{
	struct ast_node *root;
	if (argc != 2) {
		printf("incorrect command line arguments\n");
		return 1;
	}
	root = parse_program(argv[1]);
	print_tree(root);
	return 0;
}
*/

