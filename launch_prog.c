#include "launch_prog.h"

static char **make_ar_param(const word_list_t *list, int len)
{
  char **res = malloc(sizeof(char *) * (len + 1));
  char **tmp = res;
  while(list && !word_list_is_divider(list)) {
    *tmp = list->word;
    list = list->next;
    tmp++;
  }
  *tmp = NULL;
  
  return res;
}

void print_ar_param(char* const *params, int len)
{
  for(int i = 0; i <= len; i++) {
    printf("[%s]%s", params[i], i < len ? "  " : "\n");
  }
}

/*
static void finished_process_info(int res, int wstatus)
{
  if(res == -1) {
    fputs("[no child processes]\n", stderr);
  } else {
  if(res != -1) {
      fprintf(stdout, "[%d] finished with ", res);
      if(WIFEXITED(wstatus)) {
          fprintf(stdout, "[%d] code\n", WEXITSTATUS(wstatus));
      } else {
          fprintf(stdout, "[%d] signal\n", WTERMSIG(wstatus));
      }
  }
}
*/

void wait_fg_prog(int pid)
{
  int ok, wstatus;
  do {
    ok = wait(&wstatus);
    //finished_process_info(ok, wstatus);
  } while(ok != pid);
  
}

void wait_bg_prog()
{
  int ok, wstatus;
  do {
    ok = wait4(-1, &wstatus, WNOHANG, NULL);
    if(ok) {
      //finished_process_info(ok, wstatus);
    }
  } while(ok > 0);
}

static void is_error(const char *msg, int res, int err_code)
{
  if(res == -1) {
    perror(msg);
    exit(err_code);
  }
}

static void correct_exec_sequence(char* const *params)
{
  execvp(params[0], params);
  perror(params[0]);
  fflush(stderr);
  _exit(error_exec);
}

int launch_prog(const word_list_t *list)
{
  int param_len = word_list_len_until_divider(list);
  char **params = make_ar_param(list, param_len);

  fflush(stderr);
  int pid = fork();
  is_error("fork", pid, error_fork);
  if(pid == 0) {
    correct_exec_sequence(params);
  }

  free(params);
  return pid;
}

static void open_ar_fd(const redir_info_t *r_list,
		       int fd_ar[],
		       int ar_len)
{
  int i = 0;
  while(r_list && i < ar_len) {
    fd_ar[i] = open(r_list->pathname, r_list->flags, perms);
    is_error(r_list->pathname, fd_ar[i], error_open);
    
    r_list = r_list->next;
    i++;
  }
}

static void dup2_ar_fd(const redir_info_t *r_list,
		       const int fd_ar[],
		       int ar_len)
{
  int i = 0;
  while(r_list && i < ar_len) {
    int ok = dup2(fd_ar[i], r_list->fd);
    is_error("dup2", ok, error_dup2);
    
    r_list = r_list->next;
    i++;
  }
}

static void close_ar_fd(int fd_ar[], int ar_len)
{
  for(int i = 0; i < ar_len; i++) {
    int ok = close(fd_ar[i]);
    is_error("close", ok, error_close);
  }
}

int launch_with_redirection(const word_list_t *list,
			    const redir_info_t *r_list)
{
  int param_len = word_list_len_until_divider(list);
  char **params = make_ar_param(list, param_len);
  
  int fd_count = redir_info_len(r_list);
  int *fd_ar = malloc(sizeof(int) * fd_count);
  open_ar_fd(r_list, fd_ar, fd_count);

  fflush(stderr);
  int pid = fork();
  is_error("fork", pid, error_fork);
  if(pid == 0) {
    dup2_ar_fd(r_list, fd_ar, fd_count);
    close_ar_fd(fd_ar, fd_count);

    correct_exec_sequence(params);
  }
  close_ar_fd(fd_ar, fd_count);

  free(params);
  free(fd_ar);

  return pid;
}
