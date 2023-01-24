#ifndef DYNAMIC_WORD_INCREDIBLE
#define DYNAMIC_WORD_INCREDIBLE

#include "std_headers.h"

typedef struct dynamic_word {
  int size;
  int i;
  char *word;
} dynamic_word_t;

enum {
  buf = 16
};

void dynamic_word_init(dynamic_word_t **word);

void dynamic_word_push(dynamic_word_t **word, char c);

void dynamic_word_correct_str(dynamic_word_t **word);

void dynamic_word_del_last_char(dynamic_word_t *word);

void dynamic_word_free(dynamic_word_t **word);

#endif
