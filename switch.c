#include "switch.h"

static void init_new_word(dynamic_word_t **word, flags_t *mode)
{
  mode->is_word = 1;
  dynamic_word_init(word);
}

static void add_char_in_word(dynamic_word_t **word, flags_t *mode, char c)
{
  if(!mode->is_word) {
    init_new_word(word, mode);
  }
  dynamic_word_push(word, c);
}

void add_word_in_list(word_list_t **list,
		      dynamic_word_t **word,
		      flags_t *mode)
{
  if(mode->is_word) {
    mode->is_word = 0;
    dynamic_word_correct_str(word);
    word_list_push(list, *word, mode->is_divider);
    dynamic_word_free(word);
  }
}

static void turn_on_wrong_esc(flags_t *mode)
{
  if(mode->is_esc) {
    mode->wrong_esc = 1;
    mode->is_esc = 0;
  }
}

static void add_esc_seq_in_word(dynamic_word_t **word,
				flags_t *mode,
				char c)
{
  add_char_in_word(word, mode, c);
  mode->is_esc = 0;
}

static void add_divider_in_list(word_list_t **list,
				dynamic_word_t **word,
				flags_t *mode)
{
  add_word_in_list(list, word, mode);
  mode->is_divider = 0;
}

void case_blank(word_list_t **list,
		dynamic_word_t **word,
		flags_t *mode,
		char c)
{
  if(mode->is_esc) {
    turn_on_wrong_esc(mode);
    return ;
  }
  if(mode->is_quote) {
    add_char_in_word(word, mode, c);
    return ;
  }
  if(mode->is_divider) {
    add_divider_in_list(list, word, mode);
    return ;
  }
  add_word_in_list(list, word, mode);
}

void case_quote(word_list_t **list,
		dynamic_word_t **word,
		flags_t *mode,
		char c)
{
  if(mode->is_divider) {
    add_divider_in_list(list, word, mode);
  }
  if(mode->is_esc) {
    add_esc_seq_in_word(word, mode, c);
    return ;
  }
  if(mode->is_quote) {
    mode->is_quote = 0;
    return ;
  }
  if(!mode->is_quote) {
    mode->is_quote = 1;
    init_new_word(word, mode);
    return;
  }
}

static void take_last_char_in_prev_word(word_list_t **list,
					dynamic_word_t **word,
					flags_t *mode,
					char prev_char)
{
  dynamic_word_del_last_char(*word);
  add_word_in_list(list, word, mode);
  mode->is_divider = 1;
  add_char_in_word(word, mode, prev_char);
}

void case_output_redirection(word_list_t **list,
			     dynamic_word_t **word,
			     flags_t *mode,
			     div_flags_t *div_mode,
			     char c,
			     char prev_char)
{
  if(mode->is_esc) {
    add_esc_seq_in_word(word, mode, c);
    return ;
  }
  if(mode->is_quote) {
    add_char_in_word(word, mode, c);
    return ;
  }
  if(mode->is_divider) {
    if(prev_char == '&') { 
      div_mode->bg_launch--;
    }
    if(prev_char == '>') {
      add_char_in_word(word, mode, c);
      add_divider_in_list(list, word, mode);
      return;
    }
    add_char_in_word(word, mode, c);
  }
  if(!mode->is_divider) {
    if(prev_char == '1' || prev_char == '2') {
      take_last_char_in_prev_word(list, word, mode, prev_char);
    } else {
      add_word_in_list(list, word, mode);
    }
    mode->is_divider = 1;
    add_char_in_word(word, mode, c);
  }
  div_mode->output_redirect++;
}

void case_input_redirection(word_list_t **list,
			    dynamic_word_t **word,
			    flags_t *mode,
			    div_flags_t *div_mode,
			    char c)
{
  if(mode->is_esc) {
    add_esc_seq_in_word(word, mode, c);
    return ;
  }
  if(mode->is_quote) {
    add_char_in_word(word, mode, c);
    return ;
  }
  add_word_in_list(list, word, mode);

  mode->is_divider = 1;
  add_char_in_word(word, mode, c);
  add_divider_in_list(list, word, mode);
  div_mode->input_redirect++;
}

void case_ampersand(word_list_t **list,
		    dynamic_word_t **word,
		    flags_t *mode,
		    div_flags_t *div_mode,
		    char c)
{
  if(mode->is_esc) {
    add_esc_seq_in_word(word, mode, c);
    return ;
  }
  if(mode->is_quote) {
    add_char_in_word(word, mode, c);
    return ;
  }
  
  add_word_in_list(list, word, mode);
  mode->is_divider = 1;
  add_char_in_word(word, mode, c);
  div_mode->bg_launch++;
}

void case_backslash(word_list_t **list,
		    dynamic_word_t **word,
		    flags_t *mode,
		    char c)
{
  if(mode->is_divider) {
    add_divider_in_list(list, word, mode);
  }
  if(!mode->is_esc) {
    mode->is_esc = 1;
    return ;
  }
  if(mode->is_esc) {
    add_esc_seq_in_word(word, mode, c);
    return ;
  }
}

void case_default(word_list_t **list,
		  dynamic_word_t **word,
		  flags_t *mode,
		  char c)
{
  if(mode->is_divider) {
    add_divider_in_list(list, word, mode);
  }
  if(mode->is_esc) {
    turn_on_wrong_esc(mode);
    return ;
  }
  add_char_in_word(word, mode, c);
}
