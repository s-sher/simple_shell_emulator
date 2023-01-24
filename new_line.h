#ifndef NEW_LINE_INCREDIBLE
#define NEW_LINE_INCREDIBLE

#include "std_headers.h"
#include "sys_calls_headers.h"
#include "word_list.h"

typedef struct flags {
  int is_word;
  int is_quote;
  int is_esc;
  int wrong_esc;
  int is_divider;
} flags_t;

typedef struct div_flags {
  int bg_launch;
  int output_redirect;
  int input_redirect;
} div_flags_t;

enum {
  pwd_buf       = 64,
  large_pwd_buf = 128,
  max_output_redirection_count = 3
};

int is_redirection(div_flags_t div_mode);

int is_wrong_line(const word_list_t *list,
		  flags_t mode,
		  div_flags_t div_mode);

void what_was_wrong(const word_list_t *list,
		    flags_t mode,
		    div_flags_t div_mode);

void print_flags(flags_t mode, div_flags_t div_mode);

void clear_flags(void *flags, int size);

const char *get_log_user_name();

void print_greetings(const char *log_user_name);

int is_changing_dir(const word_list_t *list);

void change_directory(const word_list_t *list);

#endif
