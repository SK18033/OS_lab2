/*=============================================================================

CS 50300 Operating Systems Lab2 - A Simple Shell

lab2.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h> 

#define SIZE 100000


/*======================================================================================================
 The below method takes the indivdual commands and parses the same by tokenizing wrt spaces
 The below method returns a Character array with each item of the commands with the respective arguements 
========================================================================================================*/
char** arguments(char* cmd_str, int* cnt){

	char	str[256];
	char	*token = NULL;
	char  	**cmd_argv;
	int   	k = 0, len = 0;

	//Tokenizing the commands to get the count
	strcpy(str, cmd_str);   
	token = strtok(str, " ");
	while (token) {
	    token = strtok(NULL, " ");
	    k++;
	}

	//Creating memory to store the array of commands
	cmd_argv = (char **)malloc(sizeof(char *) * k );

	//printf("cmdargv : %s\n", cmd_str);

	strcpy(str, cmd_str);   
	token = strtok(str, " ");
	len = strlen(token);	
	k = 0;

	while (token) {
		//Creating memory to store the array of commands
		cmd_argv[k] = (char *)malloc(sizeof(char)* len);
		//printf("token: %s\n\n", token);
		strcpy(cmd_argv[k], token);   
		token = strtok(NULL, " ");

		//printf("s[%d]:%s\n\n", k, cmd_argv[k]);
		k++;
		if(token) 
			len = strlen(token);	
	}

	cmd_argv[k] = '\0';
	k++;
	*cnt = k;

	return cmd_argv;

}

/*====================================================================================
 The below method takes the command buffer and parses the same by tokenizing with pipe
 The below method returns a Character array with each item of the commands
====================================================================================*/
 
char** cmdline_parsing(char * command, int * cmd_count){

	int 	i = 0 , j = 0;
	int 	len = 0, space_cnt = 0;
	char 	*token, **cmd, spaced[SIZE]; 

	//Creating memory to store the array of commands
	cmd = (char **)malloc(sizeof(char **));
	
	token = strtok(command, "|");
	cmd[0] = (char *)malloc(1);
	cmd[0][0] = '\0';
	i++;

	while (token != NULL ) {
		
		// Removing blank spaces
		len = strlen(token);
		while(token[0] == ' '){
			//printf("fspace: %c\n", *(token));
			*token++;
		}

		space_cnt = 0;
		len = strlen(token);
		j = len-1;

		while(j >= 0){
			if(token[j] == ' ')
				space_cnt++;
			else
				break;
			j--;
		}

		j=0;
		// Creating char bytes without blank spaces
		cmd[i] = (char *)malloc(strlen(token) - space_cnt + 1);

		// Tokenizing the commands w.r.t pipe
		while(token[j] != '\0' && j <= (len-space_cnt) ){
			cmd[i][j] = token[j];
			j++;
		}
		cmd[i][j] = '\0';
		token = strtok(NULL, "|");
		//printf("token: %s\n",token);
		i++;

	}

	// Setting the arguement counts to the number of commands
	*cmd_count = i;

	return cmd;
}



/*
 The below method takes the commands and executes the commands with a pipe
 The method creates 2 pipes the structure of the fork is as shown

	fork()
		if{
			child  --> executes cmd1
	      	}
		else{  	--> Parent

			forloop(){
				fork()
				if{
					child2 --> cmd2 --- cmdn
				}
				else{ --> other Parents
					wait --> untill the child2 completes
				}
			}
		}



*/


/*
void * cmd_executor(char** cmd_tokens, int token_cnt){

	int 	dummy2, dummy1, j=0,idx= 0;
	int 	pipefd1[2], pipefd2[2] ;

	char* 	cmd1_argv = {"cat        pipe4.c  "}; 	  
	char* 	cmd2_argv = {"sort"};
	char* 	cmd3_argv = {"wc -l"};
	char* 	cmd4_argv = {"wc -l"}; 	  
	char 	**arg;
	int 	args;
	int 	PID1, PID2;

	arg = (char **)malloc(sizeof(char **));

	if (pipe(pipefd1) < 0){
		perror("pipe");
		exit(1);
	}

	PID1 = fork();
	if (PID1 < 0){
		perror("fork");
		exit(2);
	}

	if (PID1 == 0){

		// First Child

		close(pipefd1[0]);
		//Reads writes to STDOUT
		if (dup2(pipefd1[1], 1) != 1) {
			//perror("dup2");
			exit(1);
		}

		close(pipefd1[1]);
		printf(" Child\n");

		arg = arguments(cmd1_argv, &args);			
		//j = 0; while(j < args) { printf("\narg[%d]:%s\n",j, arg[j]);j++;}
		if(execvp(arg[0], arg) < 0) {
			perror("execvp"); 
			exit(1);
		} 
	}
	else{
		//Parent Process
		if (pipe(pipefd2) < 0){
			perror("pipe");
			exit(11);
		}

		for (j = 0; j < 1 ; j++ ){

			PID2=fork();

			if(PID2 < 0){
				perror("Loop fork");
				exit(22);
			}

			if (PID2 == 0){
				//Second Child
				printf("Second Child\n");

				if(idx%2 == 0){
					close(pipefd1[1]);
					close(pipefd2[0]);

					if (dup2(pipefd2[1], 1) != 1){
						//perror("dup2");
						exit(1);
					}
					close(pipefd2[1]);

					if (dup2(pipefd1[0], 0) != 0) {
						//perror("dup2");
						exit(1);
					}
					close(pipefd1[0]);
				}
				else{
					close(pipefd1[0]);
					close(pipefd2[1]);

					if (dup2(pipefd2[0], 0) != 0){
						//perror("dup2");
						exit(1);
					}
					close(pipefd2[0]);

					if (dup2(pipefd1[1], 1) != 1) {
						//perror("dup2");
						exit(1);
					}
					close(pipefd1[1]);

				}

				arg = arguments(cmd2_argv, &args);
				//j = 0; while(j < args){ printf("\narg[%d]:%s\n",j, arg[j]);j++;}
				if(execvp(arg[0], arg) < 0){
					perror("execvp"); exit(1);
				}

			}
			else{
				// Parent Processes
				wait(&dummy1);
				close(pipefd1[1]);
				close(pipefd1[0]);
				close (pipefd2[0]);
				close(pipefd2[1]);
				printf("Parent\n");
			}	
			idx++;
		}
	}
	return 0;
}
*/


int main (){

	while (1){	

		int	i = 0, cmd_count = 0 ;
		char	cmd_line[SIZE], **cmds;

		printf("\nKSooji>");

		// Read the input as the command line for the linux Shell
		while( fgets(cmd_line, SIZE , stdin) != NULL){
			//printf("Buffer\n");
        		//printf("%s\n", cmd_line);
			break;
    		}

		// Allocating memory to get the commands stored
		cmds = (char **)malloc(sizeof(char **));


		// Cmds recieves the tokenized commands wrt '|'
		cmds = cmdline_parsing(cmd_line,&cmd_count);			


		printf("Arguement Count: %d\n", cmd_count);
		i = 1; while(i < cmd_count) { printf("cmd[%d]:%s\n",i, cmds[i]);i++;}

		//cmd_executor(cmds, cmd_count);
	
	}
	return 0;

}

