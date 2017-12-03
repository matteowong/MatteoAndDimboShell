# MatteoAndDimboShell

Features that are implemented in our shell:
  Reads the input a line at a time and parses it
  Any unneccesary whitespace is removed
  Executes regular commands
  Executes cd commands
  Executes piped commands
  Executing exit
  Executes commands with simple redirection
Features we wished to implement but could not
  Complex redirection
Bugs we encountered:
  On Mac our commands file would produce the prompt ad infinitum if there was no newline at the end, this problem does not occur on Linux
  at the end of a list of commands running the same amount of prompts will be produced (you said that this was fine)
Files/Function Headers:
  parse.c
  //char * trim_white(char * s)
  //pre: takes a character pointer
  //post: returns pointer to first non-white space character and makes all terminating white space into null pointers (trims white  space)

  //char * read_buff()
  //pre: none
  //post: reads in and returns a string of up to 255 characters from standard in, replaces new line with NULL

  //int num_tokens(char * s, char * delim)
  //pre: takes a string and delimeter (single char char*)
  //post: returns number of tokens separated by the delimeter in given string

  //char ** parse_args(char * line, char * delim)
  //pre: takes a string separated by delimeter (also a string, single char)
  //post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single   delim in the input line)
  shell.c
  
