#ifndef PARSE_H
#define PARSE_H
char * trim_white(char *s);
char * read_buff();
int num_tokens(char *, char *);
char ** parse_args(char *, char*);
int array_of_str_len(char **);
int redirect(char *, char *);
int if_redirect(char ** s);
int if_pipe(char **);
void our_pipe(char **);
int execute();
#endif
