#ifndef REDIRECTION_INCREDIBLE
#define REDIRECTION_INCREDIBLE

#include "std_headers.h"
#include "sys_calls_headers.h"
#include "word_list.h"

typedef enum fd_mode {
  std_in      = STDIN_FILENO,
  std_out     = STDOUT_FILENO,
  std_err     = STDERR_FILENO,
  std_out_err = 3,
  error_fd    = -1
} fd_mode_t;

typedef struct redir_info {
  int fd;
  const char *pathname;
  int flags;
  struct redir_info *next;
} redir_info_t;

void redir_info_print(const redir_info_t *r_list);

void redir_info_free(redir_info_t **r_list);

int redir_info_len(const redir_info_t *r_list);

void fill_in_redir_info(redir_info_t **r_list, const word_list_t *list);

#endif
