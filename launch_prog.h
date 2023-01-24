#ifndef LAUNCH_PROG_INCREDIBLE
#define LAUNCH_PROG_INCREDIBLE

#include "std_headers.h"
#include "sys_calls_headers.h"
#include "word_list.h"
#include "redirection.h"

enum {
  error_open  = 1,
  error_fork  = 2,
  error_exec  = 3,
  error_close = 4,
  error_dup2  = 5,
  perms       = 0666 // rwrwrw
};

void print_ar_param(char* const *params, int len);

void wait_fg_prog(int pid);

void wait_bg_prog();

int launch_prog(const word_list_t *list);

int launch_with_redirection(const word_list_t *list,
			    const redir_info_t *r_list);

#endif
