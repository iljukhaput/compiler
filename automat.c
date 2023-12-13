#include <stdio.h>
#include <stdlib.h>
#include "automat.h"

enum { buf_size = 256, start_size_arr = 20, hash_len = 10 };

char *str_token[] = { "error", "open brace", "close brace", "open parenthesis",
						 "close parenthesis", "semicolon", "int keyword",
						 "return keyword", "identifier", "int literal" };

struct transition {
	int (*fpte)(char);
	struct state *next_state;
};

struct state {
	int num;
	struct transition *transition;
	enum token_id token_id;
};

int compare_1_9(char c)
{
	if (c >= '1' && c <= '9') {
		return 1;
	}
	return 0;
}

int compare_0(char c)
{
	return c == '0';
}

int compare_0_9(char c)
{
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

int compare_punct_mark(char c)
{
	if (c == '{' || c == '}' || c == '(' || c == ')' || c == ';') {
		return 1;
	}
	return 0;
}

int compare_word_begin(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_') {
		return 1;
	}
	return 0;
}

int compare_word_continue(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

int error_char(char c)
{

	if (c == ' ' || c == '\n' || c == '\t' || compare_word_continue(c)) {
		return 0;
	}
	return 1;
}
	
int bypass(const char *filename, struct state *s0, struct arr_t **arr, struct hash_t *h)
{
	char c;
	char *str_token;
	enum token_id token_id;
	int i = 0, trans_ok, count = 0, arr_add_ok;
	char buf[buf_size] = { 0 };
	struct state *s = s0;
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror(filename);
		exit(1);
	}

	while ((c = fgetc(file)) != EOF) {
		trans_ok = 0;
		for (i = 0; s->transition[i].fpte != NULL; i++) {
			trans_ok = s->transition[i].fpte(c);
			if (trans_ok) {
				buf[count] = c;
				count++;
				s = s->transition[i].next_state;
				break;
			}
		}
		if (!trans_ok) { // when there are no transitions in the current state for a symbol
			str_token = malloc(sizeof(char) * count);
			for (i = 0; i < count; i++) {
				str_token[i] = buf[i];
			}
			token_id = define_token_id(h, str_token, count);
			if (token_id) {
				arr_add_ok = arr_add(arr, str_token, count, token_id);
			} else {
				arr_add_ok = arr_add(arr, str_token, count, s->token_id);
			}
			if (arr_add_ok < 0) {
				return -1;
			}
			s = s0;
			count = 0;
			if (c != ' ' && c != '\n' && c != '\t') {
				ungetc(c, file);
			}
		}
	}
	return 0;
}

struct state *create_automat()
{
	struct state *s0 = malloc(sizeof(struct state));
	struct state *s1 = malloc(sizeof(struct state));
	struct state *s2 = malloc(sizeof(struct state));
	struct state *s3 = malloc(sizeof(struct state));
	struct state *s4 = malloc(sizeof(struct state));
	struct state *se = malloc(sizeof(struct state));

	struct transition *t01 = malloc(sizeof(struct transition));
	struct transition *t02 = malloc(sizeof(struct transition));
	struct transition *t03 = malloc(sizeof(struct transition));
	struct transition *t04 = malloc(sizeof(struct transition));
	struct transition *t0e = malloc(sizeof(struct transition));
	struct transition *t22 = malloc(sizeof(struct transition));
	struct transition *t33 = malloc(sizeof(struct transition));
	struct transition *tee = malloc(sizeof(struct transition));
	struct transition *te = malloc(sizeof(struct transition)); // transition end - it needs to loop

	t01->fpte = compare_0;
	t02->fpte = compare_1_9;
	t03->fpte = compare_word_begin;
	t04->fpte = compare_punct_mark;
	t0e->fpte = error_char;
	t22->fpte = compare_0_9;
	t33->fpte = compare_word_continue;
	tee->fpte = error_char;
	te->fpte = NULL;

	t01->next_state = s1;
	t02->next_state = s2;
	t03->next_state = s3;
	t04->next_state = s4;
	t0e->next_state = se;
	t22->next_state = s2;
	t33->next_state = s3;
	tee->next_state = se;
	te->next_state = NULL;

	struct transition *tarr_s0 = malloc(sizeof(struct transition) * 5);
	tarr_s0[0] = *t01;
	tarr_s0[1] = *t02;
	tarr_s0[2] = *t03;
	tarr_s0[3] = *t04;
	tarr_s0[4] = *t0e;

	struct transition *tarr_s1 = malloc(sizeof(struct transition));
	tarr_s1[0] = *te;

	struct transition *tarr_s2 = malloc(sizeof(struct transition) * 2);
	tarr_s2[0] = *t22;
	tarr_s2[1] = *te;

	struct transition *tarr_s3 = malloc(sizeof(struct transition) * 2);
	tarr_s3[0] = *t33;
	tarr_s3[1] = *te;

	struct transition *tarr_s4 = malloc(sizeof(struct transition));
	tarr_s4[0] = *te;

	struct transition *tarr_se = malloc(sizeof(struct transition) * 2);
	tarr_se[0] = *tee;
	tarr_se[1] = *te;

	s0->num = 0;
	s0->transition = tarr_s0;
	s0->token_id = 0;

	s1->num = 1;
	s1->transition = tarr_s1;
	s1->token_id = int_literal;

	s2->num = 2;
	s2->transition = tarr_s2;
	s2->token_id = int_literal;

	s3->num = 3;
	s3->transition = tarr_s3;
	s3->token_id = identifier;

	s4->num = 4;
	s4->transition = tarr_s4;
	s4->token_id = punct_mark;

	se->num = -1;
	se->transition = tarr_se;
	se->token_id = 0;

	return s0;
}

struct arr_t *get_token_arr(char *filename)
{
	int token;
	struct state *s0 = create_automat();
	struct arr_t *token_arr = arr_create(start_size_arr);
	struct hash_t *htable_token_id = htable_create(hash_len);
	bypass(filename, s0, &token_arr, htable_token_id);
	return token_arr;
}

/*
int main(int argc, char **argv) 
{

	if (argc != 2) {
		printf("incorrect command line arguments\n");
		return 1;
	}
	struct arr_t *token_arr = get_token_arr(argv[1]);
	arr_print(token_arr);
	return 0;
}
*/

