#ifndef SWITCH_INCREDIBLE
#define SWITCH_INCREDIBLE

#include "dynamic_word.h"
#include "new_line.h"
#include "std_headers.h"
#include "sys_calls_headers.h"
#include "word_list.h"

void add_word_in_list(word_list_t **list,
		      dynamic_word_t **word,
		      flags_t *mode);

void case_blank(word_list_t **list,
		dynamic_word_t **word,
	        flags_t *mode,
		char c);

void case_quote(word_list_t **list,
		dynamic_word_t **word,
		flags_t *mode,
		char c);

void case_output_redirection(word_list_t **list,
		 	     dynamic_word_t **word,
			     flags_t *mode,
			     div_flags_t *div_mode,
			     char c,
			     char prev_char);

void case_input_redirection(word_list_t **list,
			    dynamic_word_t **word,
			    flags_t *mode,
			    div_flags_t *div_mode,
			    char c);

void case_ampersand(word_list_t **list,
		    dynamic_word_t **word,
		    flags_t *mode,
		    div_flags_t *div_mode,
		    char c);

void case_backslash(word_list_t **list,
		    dynamic_word_t **word,
		    flags_t *mode,
		    char c);

void case_default(word_list_t **list,
		  dynamic_word_t **word,
		  flags_t *mode,
		  char c);

#endif
