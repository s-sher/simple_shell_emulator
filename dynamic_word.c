#include "dynamic_word.h"

void dynamic_word_init(dynamic_word_t **word)
{
  *word = malloc(sizeof(dynamic_word_t));
  (*word)->size = buf;
  (*word)->i = 0;
  (*word)->word = malloc(buf);
}

/*
static void dynamic_word_print_stat(const dynamic_word_t *word)
{
  printf("[memory %d] [filled %d] {%s}\n",
	 word->size, word->i, word->word);
}
*/

static int is_enough_mem(const dynamic_word_t *word)
{
  return (word->size) > (word->i);
}

static void dynamic_word_allocate(dynamic_word_t **word)
{
  (*word)->size += buf;
  char *tmp = malloc((*word)->size);
  strcpy(tmp, (*word)->word);
  free((*word)->word);
  (*word)->word = tmp;
}

void dynamic_word_push(dynamic_word_t **word, char c)
{
  if(!is_enough_mem(*word)) {
    dynamic_word_allocate(word);
  }
  ((*word)->word)[(*word)->i] = c;
  (*word)->i++;
}

void dynamic_word_correct_str(dynamic_word_t **word)
{
  dynamic_word_push(word, '\0');
}

void dynamic_word_del_last_char(dynamic_word_t *word)
{
  word->word[word->i - 1] = '\0';
}

void dynamic_word_free(dynamic_word_t **word)
{
  free((*word)->word);
  free(*word);
}
