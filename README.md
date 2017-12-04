# MatteoAndDimboShell by Matteo Wong and Dmytro Hvirtsman

Features that are implemented in our shell:

  * Reads the input a line at a time and parses it
  
  * Any unneccesary whitespace is removed
  
  * Executes regular commands
  
  * Executes cd commands
  
  * Executes piped commands (single)
  
  * Executing exit
  
  * Executes commands with simple redirection (only < and >, single)
	
	
Features we wished to implement but could not:

  * Complex redirection

  * Allowing multiple white spaces between commands
	
	
	
Bugs we encountered:



  * On Linux our commands file would produce the prompt ad infinitum if there was no newline at the end, this problem does not occur on Mac (eg. when testing with ./shell < commands on linux, there needs to be a new line [ENTER] at the end of the commands file). This was extremely frustrating, we spent several hours trying to get around it but could not. A similar infinite loop happens if trying to put input not through the command line and there is no new line character
  
  * at the end of a list of commands running the same amount of prompts will be produced (you said that this was fine)
  
  
Files/Function Headers:

  * parse.c
  ```c
  	//char * trim_white(char * s)
  	//pre: takes a character pointer
  	//post: returns pointer to first non-white space character and makes all terminating white space into null pointers (trims white  space)
	//looks for white space (space or new line) and sets to null or updates where pointer starts

  	//char * read_buff()
  	//pre: none
  	//post: reads in and returns a string of up to 255 characters from standard in, replaces new line with NULL
	//reads from buffer using fgets, limited to 255 characters, allocates space with calloc to ensure terminating null

  	//int num_tokens(char * s, char * delim)
  	//pre: takes a string and delimeter (single char char*)
  	//post: returns number of tokens separated by the delimeter in given string
	//starts at 1 and increments at every occurrence in a for loop that goines by character

  	//char ** parse_args(char * line, char * delim)
  	//pre: takes a string separated by delimeter (also a string, single char)
  	//post: returns an array of strings, each index holding one token from the inputted line (each token is separated by a single   delim in the input line)
	//allocates space based on size of line using calloc to ensure terminating null, then goes through and uses strsep
  
  	//int array_of_str_len(char ** s)
  	//pre: array of strings
  	//post: returns the number of strings/tokens in the array (ie. {"hi","bye"} -> 2
	//functions basically like strlen would but with an array of pointers
  
  	//int redirect(char * symbol, char * file)
  	//pre: given the symbol for redirecting (> or <) and the path to file that will be redirected to/from
	//what function does: redirects file to stdin or stdout. First sees if symbol is > or <, then creates a duplicate with dup() and saves the new file descriptor, then uses dup2, then returns new fd
  	//post: returns file descriptor to the std file that was replaced (stdout or stdin)
  
  	//int if_redirect(char ** s)
  	//pre: takes an array of strings
  	//post: returns index of redirection symbol (< or >) if the given segment requires redirecting. assumes redirect symbol isn't in 0th index
	//checks if file needs redirecting by searching for > and <
  
  	//int if_pipe(char ** s)
  	//pre: array of strings
  	//post: returns index of pipe ("|") or 0 if not there (assumes pipe isn't in 0th index)
	//checks if file needs piping by looking for |
  
  	//void our_pipe(char ** args)
  	//pre: takes an array of strings that has a pipe in it in format <command 1> | <command 2>
	//what function does: opens to pipes with popen. reads output of command 1 from the first pipe, fp_r, into a string, then reads the string, s,  into fp_w, which passes s to command 2. 
  	//post: void
  
  	//int execute()
  	//pre: none
	//splits input from buffer on semicolon, then splits again based on white space, then executes - detailed explanation in code but basically once it is split on the white space it checks if it needs to change directory, exit, pipe, or redirect, and if so does those, if not executes normally
  	//post: returns integer just b/c it has to for the main/for exit
```
  
  * shell.c
  ```c
  	//int main()
  	//Runs execute from parse.c
  ```
Final Note:

  We would like you to be aware that because of several circumstances including not being able to clone this repo from his school computer, being absent for several days, and code being sent through other venues, the work done for this project by Dmytro Hvirtsman, which is a significant fraction of the overall work, is under-represented in the github statistics. This note was approved and encouraged by both Dmytro Hvirtsman and Matteo Wong.
  
