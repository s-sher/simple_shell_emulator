#ifndef WORD_LIST_INCREDIBLE
#define WORD_LIST_INCREDIBLE

#include "std_headers.h"
#include "dynamic_word.h"

typedef struct word_list {
  char *word;
  int divider;
  struct word_list *next;
} word_list_t;

void word_list_init(word_list_t **list);

void word_list_print(const word_list_t *list);

void word_list_stat(const word_list_t *list);

void word_list_print_full(const word_list_t *list);

void word_list_push(word_list_t **list, dynamic_word_t *word, int div);

void word_list_free(word_list_t **list);

void word_list_del_empty_words(word_list_t **list);

int word_list_is_divider(const word_list_t *item);

int word_list_is_divider_str(const word_list_t *item,
			     const char *str);

int word_list_is_next_item(const word_list_t *item);

int word_list_len(const word_list_t *list);

int word_list_len_until_divider(const word_list_t *list);

const word_list_t *word_list_right_item(const word_list_t *list);

const word_list_t *word_list_get_divider(const word_list_t *list,
					 const char *str);

#endif
