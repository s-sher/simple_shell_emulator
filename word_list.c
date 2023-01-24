#include "word_list.h"

void word_list_init(word_list_t **list)
{
  *list = NULL;
}

void word_list_print(const word_list_t *list)
{
  for( ; list; list = list->next) {
    printf("[%s] ", list->word);
  }
  fputc('\n', stdout);
}

void word_list_stat(const word_list_t *list)
{
  printf(" [%14p] [%14p] [%d] [%2ld] [%s]\n",
	 list, list->next, list->divider, strlen(list->word), list->word);
}

void word_list_print_full(const word_list_t *list)
{
  for( ; list; list = list->next) {
    word_list_stat(list);
  }
}

static void word_list_push_base(word_list_t **list,
				dynamic_word_t *word,
				int div)
{
  word_list_t *tmp = malloc(sizeof(word_list_t));
  tmp->divider = div;
  tmp->word = malloc(word->i + 1);
  strcpy(tmp->word, word->word);
  tmp->next = *list;
  *list = tmp;
}

void word_list_push(word_list_t **list, dynamic_word_t *word, int div)
{
  if(!*list) {
    word_list_push_base(list, word, div);
  } else {
    word_list_push(&(*list)->next, word, div);
  }
}

static void word_list_del_item(word_list_t **item)
{
  if(!*item) {
    return ;
  }
  
  word_list_t *tmp = *item;
  *item = (*item)->next;
  free(tmp->word);
  free(tmp);
}

void word_list_free(word_list_t **list)
{
  for( ; *list; *list = (*list)->next) {
    word_list_t *tmp = *list;
    free(tmp->word);
    free(tmp);
  }
}

static int word_list_is_empty_word(const word_list_t *item)
{
  return item && strlen(item->word) == 0;
}

void word_list_del_empty_words(word_list_t **list)
{
  while(*list) {
    if(word_list_is_empty_word(*list)) {
      word_list_del_item(list);
    } else {
      list = &(*list)->next;
    }
  }
}

int word_list_is_divider(const word_list_t *item)
{
  return item && item->divider;
}

int word_list_is_divider_str(const word_list_t *item,
			     const char *str)
{
  return word_list_is_divider(item) && strstr(item->word, str);
}

int word_list_is_next_item(const word_list_t *item)
{
  return item && item->next && !word_list_is_divider(item->next);
}

int word_list_len(const word_list_t *list)
{
  int len;
  for(len = 0; list; list = list->next) {
    len++;
  }
  return len;
}

int word_list_len_until_divider(const word_list_t *list)
{
  int len = 0;
  while(list && !word_list_is_divider(list)) {
    len++;
    list = list->next;
  }
  return len;
}

const word_list_t *word_list_right_item(const word_list_t *list)
{
  const word_list_t *tmp;
  for(tmp = list; list; list = list->next) {
    tmp = list;
  }
  return tmp;
}

const word_list_t *word_list_get_divider(const word_list_t *list,
					 const char *str)
{
  for( ; list; list = list->next) {
    if(word_list_is_divider_str(list, str)) {
      return list;
    }
  }
  return NULL;
}
