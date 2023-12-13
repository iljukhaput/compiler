#ifndef _PARSER_
#define _PARSER_

struct ast_node;

struct ast_node *parse_program(char *filename);
void print_tree(struct ast_node *root);

#endif // _PARSER_
