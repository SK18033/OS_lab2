/*=============================================================================

CS 50300 Operating Systems Lab2 - A Simple Shell

lab2.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/


The purpose of the lab was to simulate a simple Linux shell using the system calls that linux provides
few of them are
1) fork()	- Creates new independent processes
2) pipe()	- Creates a new pipe with read and write ends
3) dup2()	- duplicates the file descriptors of the STDIN and STDOUT
4) execvp()	- Executes the commands with the arguements
5) Wait()	- Parent process waits for its child to finish 

There are 3 functionalities that are supported by the program

1) char** arguments(char* cmd_str, int* cnt)

	The method takes the indivdual commands and parses the same by tokenizing wrt spaces. The method returns a Character array with each item of the commands with the respective arguements. 


2) char** cmdline_parsing(char * command, int * cmd_count)

	The method takes the command buffer and parses the same by tokenizing with respect to pipe. The below method returns a Character array with list of the individual commands


3) void * cmd_executor(char** cmd_tokens, int token_cnt){

	 The method takes the commands and executes the commands with a pipe. The method creates 2 pipes the structure of the fork(), where the child executes the the first command and the parent forks another process that executes the remaining commands.


NOTE : The loop is running only for one pipe as in j = 1. I have tried to execute for the multiple pipes. There is an issue with read write heads of the pipe.

Exceution Steps :

1) Change the current working directory to the source code location
2) Make sure there is a ".c" file file
3) type makefile and hit enter
4) Once the program is done run make clean 
