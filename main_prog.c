#include "std_headers.h"
#include "dynamic_word.h"
#include "word_list.h"
#include "launch_prog.h"
#include "new_line.h"
#include "switch.h"
#include "redirection.h"

int main()
{
  word_list_t *list;
  dynamic_word_t *word;
  
  flags_t mode = {0};
  div_flags_t div_mode = {0};
  
  const char *user_name;
  char prev_char;
  int c, pid;
  
  word_list_init(&list);
  user_name = get_log_user_name();
  print_greetings(user_name);
  
  while((c = getchar()) != EOF) {
    switch(c) {
      
    case '\n':
      if(mode.is_word) {
        add_word_in_list(&list, &word, &mode);
      }
      if(is_wrong_line(list,mode, div_mode)) {
	what_was_wrong(list, mode, div_mode);
	goto clear_memory;
      }
      if(list) {
	word_list_del_empty_words(&list);
	if(is_changing_dir(list)) {
	  change_directory(list);
	  goto clear_memory;
	}
	if(is_redirection(div_mode)) {
	  redir_info_t *redir_list;
	  fill_in_redir_info(&redir_list, list);
	  pid = launch_with_redirection(list, redir_list);
	  redir_info_free(&redir_list);
	} else { 
	  pid = launch_prog(list);
	}
	if(!div_mode.bg_launch) {
	  wait_fg_prog(pid);
	}
	wait_bg_prog();
      }
      
      //word_list_print_full(list);
    clear_memory:
      clear_flags(&mode, sizeof(mode));
      clear_flags(&div_mode, sizeof(div_mode));
      word_list_free(&list);
      
      wait_bg_prog();
      print_greetings(user_name);
      break;
      
    case  ' ':
    case '\t':
      case_blank(&list, &word, &mode, c);
      break;

    case '\"':
      case_quote(&list, &word, &mode, c);
      break;

    case '>':
      case_output_redirection(&list, &word, &mode, &div_mode, c, prev_char);
      break;
      
    case '<':
      case_input_redirection(&list, &word, &mode, &div_mode, c);
      break;

    case '&':
      case_ampersand(&list, &word, &mode, &div_mode, c);
      break;
      
    case '\\':
      case_backslash(&list, &word, &mode, c);
      break;
      
    default:
      case_default(&list, &word, &mode, c);
      break;
    }
    prev_char = c;
  }
  
  return 0;
}
