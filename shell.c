#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//read_buff()
//pre: none
//post: reads in and returns a string of up to 255 characters from standard in, replaces new line with NULL
char * read_buff() {

  char * s=(char *)calloc(256,1);
  fgets(s,256,stdin);
  //printf("%s\n",s);
  s[strlen(s)-1]=0;//remove new line
  return s;
}

//num_tokens
//pre: takes a string
//post: returns number of tokens separated by a space in given string
int num_tokens(char * s){
  //printf("s is %s\n",s);
  int ret=1;
  int i=0;
  for(;i<strlen(s);i++) {
    if(s[i]==' ') {
      ret++;
      //printf("added %d\n",i);
    }
  }
  return ret;


}

//parse_args
//pre: takes a string separated by spaces
//post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single space in the input line)
char** parse_args(char * line) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens(line)+1);
  //printf("line: %s\n",line);
  int i=0;
  while (line){
    s[i]=line;
    //printf("line before strsep: %s\n",line);
    strsep(&line, " ");
    //printf("line after strsep: %s\n",line);
    i++;
  }
  return s;


}

int main() {

  int status;
  while (1) {
  char ** args=parse_args(read_buff());
  if (fork()==0) {
    if (!strcmp(args[0],"exit")) {
      return 1;
    }
    execvp(args[0],args);
  }
  else {
    wait(&status);
    if (WEXITSTATUS(status)==1)//exits
      return 0;
  }

  }
  
  return 0;
}
