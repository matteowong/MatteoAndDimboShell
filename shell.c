#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

char * read_buff() {

  char * s=(char *)calloc(256,1);
  fgets(s,256,stdin);
  //printf("%s\n",s);
  s[strlen(s)-1]=0;//remove new line
  return s;
}

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

char** parse_args(char * line) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens(line)+1);
  printf("line: %s\n",line);
  int i=0;
  while (line){
    s[i]=line;
    printf("line before strsep: %s\n",line);
    strsep(&line, " ");
    printf("line after strsep: %s\n",line);
    i++;
  }
  return s;


}

int main() {

  char ** args=parse_args(read_buff());
  /*int i=0;
  while (args[i]) {
    printf("%d: %s\n",i,args[i]);
    i++;
  }
  printf("%d: %s\n",i,args[i]);
  printf("%s\n",args[0]);*/

  execvp(args[0],args);
  
  return 0;
}
