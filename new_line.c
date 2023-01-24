#include "new_line.h"

static int is_wrong_esc(flags_t mode)
{
  return mode.wrong_esc || mode.is_esc;
}

static int is_wrong_redirection(div_flags_t div_mode)
{
  return div_mode.input_redirect > 1 ||
    div_mode.output_redirect > max_output_redirection_count;
}

int is_correct_bg_launch(const word_list_t *list)
{
  const word_list_t *last_item;
  last_item = word_list_right_item(list);
  return word_list_is_divider_str(last_item, "&");
}

static int is_wrong_bg_launch(const word_list_t *list,
			      div_flags_t div_mode)
{
  return div_mode.bg_launch && !is_correct_bg_launch(list);
}

int is_redirection(div_flags_t div_mode)
{
  return div_mode.input_redirect || div_mode.output_redirect;
}

int is_wrong_line(const word_list_t *list,
		  flags_t mode,
		  div_flags_t div_mode)
{
  return is_wrong_esc(mode) ||
    mode.is_quote ||
    is_wrong_redirection(div_mode) ||
    div_mode.bg_launch > 1 ||
    is_wrong_bg_launch(list, div_mode);
}

void what_was_wrong(const word_list_t *list,
		    flags_t mode,
		    div_flags_t div_mode)
{
  if(is_wrong_esc(mode)) {
    fputs("incorrect escape sequence\n", stderr);
  }
  if(mode.is_quote) {
    fputs("unmatched quotes\n", stderr);
  }
  if(div_mode.output_redirect > max_output_redirection_count) {
    fputs("incorrect number of output stream redirects\n", stderr);
  }
  if(div_mode.input_redirect > 1) {
    fputs("incorrect number of input stream redirects\n", stderr);
  }
  if(div_mode.bg_launch > 1) {
    fputs("incorrect number of background launching\n", stderr);
  }
  if(is_wrong_bg_launch(list, div_mode)) {
    fputs("incorrect background launcing position\n", stderr);
  }
}

void print_flags(flags_t mode, div_flags_t div_mode)
{
  printf(" [is_word %d] [is_quote %d]"
	 " [is_esc %d] [wrong_esc %d]"
	 "  [is_divider %d]\n",
	 mode.is_word, mode.is_quote,
	 mode.is_esc, mode.wrong_esc,
	 mode.is_divider);
  
  printf(" {bg %d} {output_redirect %d} {input redirect %d}\n",
	 div_mode.bg_launch,
	 div_mode.output_redirect,
	 div_mode.input_redirect);
}

void clear_flags(void *flags, int size)
{
  memset(flags, 0, size);
}

static void is_error_ptr(const char *res, const char *err_msg)
{
  if(!res)
    perror(err_msg);
}

const char *get_log_user_name()
{
  const char *logname = getenv("LOGNAME");
  const char *username = getenv("USER");
  is_error_ptr(logname, "log_name");
  is_error_ptr(username, "user_name");
  
  char *res = malloc(strlen(logname) + strlen(username) + 1);
  sprintf(res, "%s@%s", logname, username);
  
  return res;
}

static char *get_pwd()
{
  char *pwd = malloc(pwd_buf);
  char *ok = getcwd(pwd, pwd_buf);
  
  if(!ok) {
    free(pwd);
    pwd = malloc(large_pwd_buf);
    ok = getcwd(pwd, large_pwd_buf);
  }
  
  return ok ? pwd : ok;
}

void print_greetings(const char *log_user_name)
{
  char *pwd = get_pwd();
  is_error_ptr(pwd, "pwd");
  
  fprintf(stdout, "%s:%s$ ", log_user_name, pwd);
  free(pwd);
}

int is_changing_dir(const word_list_t *list)
{
  if(!list) {
    return 0;
  }
  return 0 == strcmp("cd", list->word);
}

void change_directory(const word_list_t *list)
{
  const char *path;
  if(list->next) {
    path = list->next->word;
  } else {
    path = getenv("HOME");
    if(!path) {
      perror("where is our home?");
      return ;
    }
  }

  int ok = chdir(path);
  if(ok == -1) {
    perror("chdir");
    return ;
  }
}
