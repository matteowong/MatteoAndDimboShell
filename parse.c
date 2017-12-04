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
//looks for white space (space or new line) and sets to null or updates where pointer starts
char * trim_white(char * s) {

  while (*s==' ') {
    s++;
  }

  int end=strlen(s)-1;
  while (s[end]==' ' || s[end] == '\n')
    s[end--]=0;
  return s;

}

//char * read_buff()
//pre: none
//post: reads in and returns a string of up to 255 characters from standard in, replaces new line with NULL
//reads from buffer using fgets, limited to 255 characters, allocates space with calloc to ensure terminating null
char * read_buff() {

  char * s=(char *)calloc(256,1);
  fgets(s,256,stdin);
  //printf("%s\n",s);
  s=trim_white(s);//remove new line
  return s;
}


//int num_tokens(char * s, char * delim)
//pre: takes a string and delimeter (single char char*)
//post: returns number of tokens separated by the delimeter in given string
//starts at 1 and increments at every occurrence in a for loop that goines by character
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

/*
attempt at separating with multiple white spaces
int num_tokens_multiple_white(char * s){
  int ret=1;
  int past=-1;
  int i=1;
  for (;i<strlen(s);i++) {
    if (s[i]== ' ' && past>=0 && s[past]!=' ') {
      ret++;
    }
  }
  return ret;
  }*/

//char ** parse_args(char * line, char * delim)
//pre: takes a string separated by delimeter (also a string, single char)
//post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single delim in the input line)
//allocates space based on size of line using calloc to ensure terminating null, then goes through and uses strsep
char** parse_args(char * line, char * delim) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens(line, delim)+1);
  //printf("line: %s\n",line);
  int i=0;
  while (line){
    //if (i==0 && *line!=*delim)
      s[i]=line;
    //printf("line before strsep: %s\n",line);
    strsep(&line, delim);
    //printf("line after strsep: %s\n",line);
    i++;
  }
  return s;


}

/*
other part of attempt to separate with multiple white spaces
char ** parse_args(char * line) {

  char ** s=(char **)calloc(sizeof(char *),num_tokens_multiple_white(line)+1);
  int i=0;
  while (line) {
    if (!line[-1])
      continue;
    else
      strsep(&line, " ");
  }

  }*/


//int array_of_str_len(char ** s)
//pre: array of strings
//post: returns the number of strings/tokens in the array (ie. {"hi","bye"} -> 2
//functions basically like strlen would but with an array of pointers
int array_of_str_len(char ** s) {
  int i=0;
  while (s[i++]);
  return --i;//subtract one because while loop adds one extra
}


//int redirect(char * symbol, char * file)
//pre: given the symbol for redirecting (> or <) and the path to file that will be redirected to/from
//what function does: redirects file to stdin or stdout. First sees if symbol is > or <, then creates a duplicate with dup() and saves the new file descriptor, then uses dup2, then returns new fd
//post: returns file descriptor to the std file that was replaced (stdout or stdin)
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
    std_copy=std_copy*-1;//negative is how we identify redirecting stdin vs stdout
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

//int if_redirect(char ** s)
//pre: takes an array of strings
//post: returns index of redirection symbol (< or >) if the given segment requires redirecting. assumes redirect symbol isn't in 0th index
//checks if file needs redirecting by searching for > and <
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

//int if_pipe(char ** s)
//pre: array of strings
//post: returns index of pipe ("|") or 0 if not there (assumes pipe isn't in 0th index)
//checks if file needs piping by looking for |
int if_pipe(char ** s) {
  int i=array_of_str_len(s)-1;
  while (i>-0) {
    if (!strcmp(s[i],"|"))
      return i;
    i--;
  }
  return 0;
}

//void our_pipe(char ** args)
//pre: takes an array of strings that has a pipe in it in format <command 1> | <command 2>
//what function does: opens to pipes with popen. reads output of command 1 from the first pipe, fp_r, into a string, then reads the string, s,  into fp_w, which passes s to command 2. 
//post: void
void our_pipe(char ** args){

  FILE * fp_r;
  FILE * fp_w;
  fp_r=popen(args[0],"r");
  fp_w=popen(args[if_pipe(args)+1],"w");
  char s[1000];
  char temp[256];
  while (fgets(temp,256,fp_r)) {
    strcat(s,temp);
    //printf("%s\n",s);
  }
  fprintf(fp_w,"%s",s);
  fclose(fp_r);
  fclose(fp_w);
  //printf("%s\n",s);
  

}


//int execute()
//pre: none
//splits input from buffer on semicolon, then splits again based on white space, then executes - detailed explanation in code but basically once it is split on the white space it checks if it needs to change directory, exit, pipe, or redirect, and if so does those, if not executes normally
//post: returns integer just b/c it has to for the main/for exit
int execute() {

  int status;
  while (1) {//infinite while loop
    printf("MatteoAndDimboShell$ ");//prompt
    char * s = read_buff();
    
    char ** args=parse_args(s,";");//parse on semicolon
    
    int total=array_of_str_len(args);//number of commands separated by semicolon
    
    //printf("total: %d\n",total);
    int i=0;
    /*while (i<total) {
      printf("%d: %s\n",i,args[i]);
      i++;
      }
      i=0;*/
    while (i<total) {//loops through all commands separated by semicolons
      char ** sub_args=parse_args(trim_white(args[i])," ");//separate ith group of commands on space
      int fd[2];//pipe for reading in
      pipe(fd);
      if (fork()==0) {//child
	close(fd[0]);//close reading
	//printf("sub_args[0]: [%s]\n",sub_args[0]);
	if (!strcmp(sub_args[0],"exit")) {//exiting
	  close(fd[1]);
       	  return 4;
	}
	else if (!strcmp(sub_args[0], "cd")) {//changing directory
	  close(fd[1]);
	  return 2;
	}else if (if_pipe(sub_args)) {//piping
	  close(fd[1]);
	  our_pipe(sub_args);
	  return 3;
	}
	else if (if_redirect(sub_args)) {//redirecting
	  int ind=if_redirect(sub_args);
	  int std_fd=redirect(sub_args[ind],sub_args[ind+1]);
	  //printf("[%s]\n",sub_args[ind+1]);
	  sub_args[ind]=0;
	  write(fd[1],&std_fd,sizeof(int));//writes what the new fd for stdin/out made through dup() is so it can be fixed later
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
      else {//parent process
	//printf ("1");
	close(fd[1]);//close writing
	wait(&status);//waits for child to finish
	if (WEXITSTATUS(status)==4){//exits
	  //printf("parse.c: exit\n");
	  //printf("exiting\n");
	  close(fd[0]);
	  free(s);
	  free(sub_args);
	  free(args);
	  return 0;
	}
	else if (WEXITSTATUS(status) == 2) {//cd
	  //printf("parse.c: changing directory\n");
	  //printf("dir to enter: [%s]\n",sub_args[1]);
	  //printf("dir to enter: [%s]\n",sub_args[2]);
	  close(fd[0]);
	  if (sub_args[1]&&chdir(sub_args[1])) {
	    printf("chdir error: %s\n",strerror(errno));
	  }
	}
	else if (WEXITSTATUS(status) == 3) {//pipe
	  /*printf("sdasd");
	  FILE *fp;
	  char path[1035];

	  fp = popen(sub_args[1], "r");
	  while (fgets(path, sizeof(path)-1, fp));
	  pclose(fp);
	  execvp(sub_args[2], path);*/
	  }
	else {//redirecting
	  int j=0;//if no redirecting it will equal 0 and skip the following things
	  read(fd[0],&j,sizeof(int));
	  if (j>0) {
	    //printf("parse.c: redirected stdout\n");
	    dup2(j,1);
	  }
	  else if (j<0) {
	    //printf("parse.c: redirected stdin\n");
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
