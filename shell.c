#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


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
//post: returns an int due to forking
//splits input from buffer on semicolon, then splits again based on white space, then executes
int execute() {

  int status;
  while (1) {
    char ** args=parse_args(read_buff(),';');
    int total=array_of_str_len(args);
    //printf("total: %d\n",total);
    int i=0;
    while (i<total) {
      char ** sub_args=parse_args(trim_white(args[i]),' ');
      if (fork()==0) {
	if (!strcmp(sub_args[0],"exit")) {
	  return 1;
	}
	execvp(sub_args[0],sub_args);
      }
      else {
	wait(&status);
	if (WEXITSTATUS(status)==1)//exits
	  return 0;
	i++;
      }
    }

  }
  return 0;
  
}

int main() {

  execute();
  return 0;
}
