#include "redirection.h"

static const char *output_sample = ">";
static const char *input_sample = "<";

static char* const std_out_sample[] = {">", "1>", ">>", "1>>", NULL};
static char* const std_err_sample[] = {"2>", "2>>", NULL};
static char* const std_out_err_sample[] = { "&>", "&>>", NULL};

static void redir_info_init(redir_info_t **r_list)
{
  *r_list = NULL;
}

static void redir_info_stat(const redir_info_t *r_list)
{
  printf(" [%14p] [%14p]"
	 " [fd %d] [flags %4d]"
	 " [name %s]\n",
	 r_list, r_list->next,
	 r_list->fd, r_list->flags,
	 r_list->pathname);
}

void redir_info_print(const redir_info_t *r_list)
{
  for( ; r_list; r_list = r_list->next) {
    redir_info_stat(r_list);
  }
}

static void redir_info_push_base(redir_info_t **r_list,
				 int fd,
				 const char *pathname,
				 int flags)
{
  redir_info_t *tmp = malloc(sizeof(redir_info_t));
  tmp->fd = fd;
  tmp->pathname = pathname;
  tmp->flags = flags;
  tmp->next = *r_list;
  *r_list = tmp;
}

static void redir_info_push(redir_info_t **r_list,
			    int fd,
			    const char *pathname,
			    int flags)
{
  if(!*r_list) {
    redir_info_push_base(r_list, fd, pathname, flags);
  } else {
    redir_info_push(&(*r_list)->next, fd, pathname, flags);
  }
}

void redir_info_free(redir_info_t **r_list)
{
  for( ; *r_list; *r_list = (*r_list)->next) {
    redir_info_t *tmp = *r_list;
    free(tmp);
  }
}

static int redir_info_len_base(const redir_info_t *r_list, int len)
{
  return r_list ? redir_info_len_base(r_list->next, len+1) : len;
}

int redir_info_len(const redir_info_t *r_list)
{
  return redir_info_len_base(r_list, 0);
}

static int is_redirect_item(const word_list_t *item)
{
  return item &&  
    (word_list_is_divider_str(item, input_sample) ||
     word_list_is_divider_str(item, output_sample));
}

static int is_stdout_fd(char* const *sample, const char *str)
{
  if(!*sample) {
    return 0;
  }
  return 0 == strcmp(str, *sample) ? 1 : is_stdout_fd(sample+1, str);
}

static fd_mode_t get_num_fd(const word_list_t *item)
{
  if(!item) {
    return error_fd;
  }
  
  const char *str = item->word;

  if(0 == strcmp(str, "<")) {
    return std_in;
  }
  if(is_stdout_fd(std_out_sample, str)) {
    return std_out;
  }
  if(is_stdout_fd(std_err_sample, str)) {
    return std_err;
  }
  if(is_stdout_fd(std_out_err_sample, str)) {
    return std_out_err;
  }
  return error_fd;
}

static int get_flags(const word_list_t *item)
{
  if(!item) {
    return -1;
  }
  
  const char *str = item->word;
  
  if(0 == strcmp(str, "<")) {         // 0
    return O_RDONLY;
  }
  if(0 == strcmp(str, "&>")) {        // 1601
    return O_WRONLY|O_CREAT|O_TRUNC|O_APPEND;
  }
  if(strstr(str, ">>")) {             // 1089
    return O_WRONLY|O_CREAT|O_APPEND;
  }
  if(strstr(str, ">")) {              // 577
    return O_WRONLY|O_CREAT|O_TRUNC;
  }
  return -1;
}

void fill_in_redir_info(redir_info_t **r_list, const word_list_t *list)
{
  fd_mode_t fd;
  const char *pathname;
  int flags;
  redir_info_init(r_list);
  
  for( ; list; list = list->next) {
    if(is_redirect_item(list)) {
      fd = get_num_fd(list);
      if(fd == std_out_err) {
	redir_info_push(r_list, STDOUT_FILENO, pathname, flags);
	redir_info_push(r_list, STDERR_FILENO, pathname, flags);
      } else {
	redir_info_push(r_list, (int)fd, pathname, flags);
      }
      pathname = word_list_is_next_item(list) ? list->next->word : NULL;
      flags = get_flags(list);
    }
  }
}
