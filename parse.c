#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

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
int num_tokens(char * s, char * delim){
  //printf("s is %s\n",s);
  int ret=1;
  int i=0;
  for(;i<strlen(s);i++) {
    if(s[i]==delim[0]) {
      ret++;
      //printf("added %d\n",i);
    }
  }
  //if (!strcmp(s,"exit"))
  //printf("exit command num tokens\n");
  return ret;


}

//parse_args
//pre: takes a string separated by spaces
//post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single space in the input line)
char** parse_args(char * line, char * delim) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens(line, delim)+1);
  //printf("line: %s\n",line);
  int i=0;
  while (line){
    s[i]=line;
    //printf("line before strsep: %s\n",line);
    strsep(&line, delim);
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


//redirect()
//pre: given the symbol for redirecting (i.e. >, >>, etc.) and the path to file that will be redirected to/from
//post: returns file descriptor to the std file that was replaced (stdout, stdin, etc)
int redirect(char * symbol, char * file) {
  //printf("redirect() started\n");
  int std_copy;
  int fd;
  if (!strcmp(symbol,">")) {
    fd=open(file,O_TRUNC | O_CREAT | O_WRONLY,0644);
    std_copy=dup(1);
    //printf("redirected [%d] to stdout\n",fd);
    dup2(fd,1);
    
  }
  else if (!strcmp(symbol,"<")) {
    //printf("redirecting stdin in redirect()\n");
    fd=open(file,O_CREAT | O_RDWR,0644);
    //printf("opened\n");
    std_copy=dup(0);
    std_copy=std_copy*-1;
    //printf("redirected [%d] to stdin\n",fd);
    dup2(fd,0);
  }
  /*else if (!strcmp(symbol,">>"))
    fd=open(file,O_APPEND | O_CREAT | O_WRONLY,0644);
  if (symbol[0]=='>') {
    std_copy=dup(1);//duplicate standard out
    dup2(fd,1);
  } else if (symbol[0]=='<') {//standard in
    printf("redirecting stdin\n");
    std_copy=-1*dup(0);
    dup2(fd,0);
    }*/
  return std_copy;
}

//returns index of redirection symbol if the given segment requires redirecting
int if_redirect(char ** s) {

  int i=array_of_str_len(s)-1;
  while (i>=0) {
    if (!(strcmp(s[i],">") /*&& strcmp(s[i],">>")*/ && strcmp(s[i],"<"))) {
      //printf("index of redirect: %d\n",i);
      return i;
    }
    i--;
  }
  return 0;

}


//execute()
//pre: none
//splits input from buffer on semicolon, then splits again based on white space, then executes
int execute() {

  int status;
  while (1) {
    //printf("MatteoAndDimboShell$ ");
    char * s=read_buff();


    char ** args=parse_args(s,";");
    
    int total=array_of_str_len(args);
    
    //printf("total: %d\n",total);
    int i=0;
    /*while (i<total) {
      printf("%d: %s\n",i,args[i]);
      i++;
      }
      i=0;*/
    while (i<total) {
      char ** sub_args=parse_args(trim_white(args[i])," ");
      int fd[2];
      pipe(fd);
      if (fork()==0) {
	close(fd[0]);//close reading
	printf("sub_args[1]: [%s]\n",sub_args[1]);
	if (!strcmp(sub_args[0],"exit")) {
	  close(fd[1]);
       	  return 1;
	}
	else if (!strcmp(sub_args[0], "cd")) {
	  close(fd[1]);
	  return 2;
	}else if (!strcmp(sub_args[1], "|")) {
	  close(fd[1]);
	  return 3;
	}
	else if (if_redirect(sub_args)) {
	  int ind=if_redirect(sub_args);
	  int std_fd=redirect(sub_args[ind],sub_args[ind+1]);
	  //printf("[%s]\n",sub_args[ind+1]);
	  sub_args[ind]=0;
	  write(fd[1],&std_fd,sizeof(int));
	  close(fd[1]);
	  //printf("execute is redirecting\n");
	  //printf("[%s] [%s]\n",sub_args[0],sub_args[1]);
	  execvp(sub_args[0],sub_args);
	  return 0;
	}
	else {
	  //printf("normal\n");
	  execvp(sub_args[0],sub_args);
	  return 0;
	}
      }
      else {
	printf ("1");
	close(fd[1]);//close writing
	//int j=0;
	//read(fd[0],&j,sizeof(int));
	printf ("2");
	wait(&status);
	printf ("3");
	//printf("%d\n",WEXITSTATUS(status));
	if (WEXITSTATUS(status)==1){//exits
	  printf("parse.c: exit\n");
	  //printf("exiting\n");
	  close(fd[0]);
	  free(s);
	  free(sub_args);
	  free(args);
	  return 0;
	}
	else if (WEXITSTATUS(status) == 2) {
	  printf("parse.c: changing directory\n");
	  //printf("dir to enter: [%s]\n",sub_args[1]);
	  //printf("dir to enter: [%s]\n",sub_args[2]);
	  close(fd[0]);
	  if (sub_args[1]&&chdir(sub_args[1])) {
	    printf("chdir error: %s\n",strerror(errno));
	  }
	}
	else if (WEXITSTATUS(status) == 3) {
	  printf("sdasd");
	  FILE *fp;
	  char path[1035];

	  fp = popen(sub_args[1], "r");
	  while (fgets(path, sizeof(path)-1, fp));
	  pclose(fp);
	  execvp(sub_args[2], path);
	}
	else {
	  int j=0;
	  read(fd[0],&j,sizeof(int));
	  if (j>0) {
	    printf("parse.c: redirected stdout\n");
	    dup2(j,1);
	  }
	  else if (j<0) {
	    printf("parse.c: redirected stdin\n");
	    j*=-1;
	    dup2(j,0);
	  }
	  close(fd[0]);
	}
	//j=0;
	free(sub_args);
	status=0;
	i++;
      }
    }
    free(s);
    free(args);

  }
  return 0;
  
}
