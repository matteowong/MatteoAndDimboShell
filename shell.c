#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>


//char * trim_white(char * s)
//pre: takes a character pointer
//post: returns pointer to first non-white space character and makes all terminating white space into null pointers (trims white space)
char * trim_white(char * s) {

  while (*s==' ') {
    s++;
  }

  int end=strlen(s)-1;
  while (s[end]==' ' || s[end] == '\n')
    s[end--]=0;
  return s;

}

//read_buff()
//pre: none
//post: reads in and returns a string of up to 255 characters from standard in, replaces new line with NULL
char * read_buff() {

  char * s=(char *)calloc(256,1);
  fgets(s,256,stdin);
  //printf("%s\n",s);
  s=trim_white(s);//remove new line
  return s;
}


//num_tokens
//pre: takes a string
//post: returns number of tokens separated by a space in given string
int num_tokens(char * s, char delim){
  //printf("s is %s\n",s);
  int ret=1;
  int i=0;
  for(;i<strlen(s);i++) {
    if(s[i]==delim) {
      ret++;
      //printf("added %d\n",i);
    }
  }
  return ret;


}

//parse_args
//pre: takes a string separated by spaces
//post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single space in the input line)
char** parse_args(char * line, char delim) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens(line, delim)+1);
  //printf("line: %s\n",line);
  int i=0;
  while (line){
    s[i]=line;
    //printf("line before strsep: %s\n",line);
    strsep(&line, &delim);
    //printf("line after strsep: %s\n",line);
    i++;
  }
  return s;


}

int array_of_str_len(char ** s) {
  int i=0;
  while (s[i++]);
  return --i;//subtract one because while loop adds one extra
}


//execute()
//pre: none
//splits input from buffer on semicolon, then splits again based on white space, then executes
int execute() {

  int status;
  while (1) {
    char ** args=parse_args(read_buff(),';');
    int total=array_of_str_len(args);
    
    //printf("total: %d\n",total);
    int i=0;
    while (i<total) {
      printf("%d: %s\n",i,args[i]);
      i++;
    }
    i=0;
    while (i<total) {
      char ** sub_args=parse_args(trim_white(args[i]),' ');
      /*int fd[2];
	pipe(fd);*/
      if (fork()==0) {
	if (!strcmp(sub_args[0],"exit")) {
	  //printf("%s\n",sub_args[0]);
	  return 1;
	}
	if (!strcmp(sub_args[0], "cd")) {
	  /*close(fd[0]);
	  char s[sizeof(sub_args[1])];
	  strcpy (s, sub_args[1]);
	  printf("%s\n", sub_args[1]);
	  printf("%s\n", s);
	  write(fd[1], s, sizeof(s));*/
	  return 2;
	}
	execvp(sub_args[0],sub_args);
      }
      else {
	wait(&status);
	//printf("%d\n",WEXITSTATUS(status));
	if (WEXITSTATUS(status)==1){//exits
	  //printf("exiting\n");
	  exit(0);
	}
	if (WEXITSTATUS(status) == 2) {
	  /*close(fd[1]);
	  char s[256];
	  read(fd[0], s, sizeof(s));
	  printf("%s\n", s);*/
	  printf("changing directory\n");
	  if (sub_args[1]&&chdir(sub_args[1])) {
	    printf("chdir error: %s\n",strerror(errno));
	  }
	}
	status=0;
	i++;
      }
    }

  }
  return 0;
  
}

int main() {

  return execute();
}

