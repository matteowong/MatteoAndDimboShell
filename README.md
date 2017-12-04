# MatteoAndDimboShell by Matteo Wong and Dmytro Hvirtsman

Features that are implemented in our shell:


  Reads the input a line at a time and parses it
  
  Any unneccesary whitespace is removed
  
  Executes regular commands
  
  Executes cd commands
  
  Executes piped commands
  
  Executing exit
  
  Executes commands with simple redirection
	
	
Features we wished to implement but could not:

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
  
  //int array_of_str_len(char ** s)
  //pre: array of strings
  //post: returns the number of strings/tokens in the array (ie. {"hi","bye"} -> 2
  
  //int redirect(char * symbol, char * file)
  //pre: given the symbol for redirecting (> or <) and the path to file that will be redirected to/from
  //what function does: redirects file to stdin or stdout
  //post: returns file descriptor to the std file that was replaced (stdout or stdin)
  
  //int if_redirect(char ** s)
  //pre: takes an array of strings
  //post: returns index of redirection symbol (< or >) if the given segment requires redirecting. assumes redirect symbol isn't in 0th index
  
  //int if_pipe(char ** s)
  //pre: array of strings
  //post: returns index of pipe ("|") or 0 if not there (assumes pipe isn't in 0th index)
  
  //void our_pipe(char ** args)
  //pre: takes an array of strings that has a pipe in it in format <command 1> | <command 2>
  //what function does: opens to pipes with popen. reads output of command 1 from the first pipe, fp_r, into a string, then reads   the string, s,  into fp_w, which passes s to command 2. 
  //post: void
  
  //int execute()
  //pre: none
  //splits input from buffer on semicolon, then splits again based on white space, then executes - detailed explanation in code
  //post: returns integer just b/c it has to for the main/for exit
  
  shell.c
  
  //int main()
  //Runs execute from parse.c
  
Final Note:
  We would like you to be aware that because of several circumstances including not being able to clone this repo from his school computer, being absent for several days, and code being sent through other venues, the work done for this project by Dmytro Hvirtsman, which is a significant fraction of the overall work, is under-represented in the github statistics. This note was approved and encouraged by both Dmytro Hvirtsman and Matteo Wong.
  
