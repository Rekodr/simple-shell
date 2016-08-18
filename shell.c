#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include <fcntl.h>




void excecute(char* cmd, int parent, int child){
	
	int shouldPipe = isPiping(cmd);
    
	if(shouldPipe == 0){
		dup2(parent, 0);
		dup2(child, 1);

		//checking for any redirection 
		char direction; 
		int shouldRedirect = isRedirecting(cmd, &direction); 
		if(shouldRedirect){
        	char **commands = splitCmd_redirection(cmd); 
        	redirect(commands, &direction);
			exit(EXIT_SUCCESS); 
		}

		char** commands = splitCmd(cmd);
		
		if(commands[0] == NULL){
			exit(EXIT_FAILURE) ; 
		}

		if( execvp(commands[0], commands) == -1){
			fprintf(stderr, "commands not found:> %s\n", commands[0]);
			exit(EXIT_FAILURE); 
		}
	}

	else{
		char* pipe_cmd = &cmd[shouldPipe + 1];
		cmd[shouldPipe] = '\0';
		int pipefd[2];
		int pipeSuccess;
		pid_t cpid;

		pipeSuccess = pipe(pipefd);
		if(pipeSuccess == -1){
			perror("pipe");
			exit(EXIT_FAILURE);
		}
 
 		cpid = fork();
 		if(cpid == -1){
 			perror("fork");
 			exit(EXIT_FAILURE);
 		}

 		if(cpid == 0){
 			//printf("piping \n");
 			close(pipefd[0]);
 			excecute(cmd, parent, pipefd[1]);
 			exit(EXIT_SUCCESS);
 		}
 		else{
 			close(pipefd[1]);
 			excecute(pipe_cmd, pipefd[0], child);
 			exit(EXIT_SUCCESS);
 		}

	}
}



int redirect(char** cmd, char* direction){

	int fd; 
	if(cmd[0] == NULL || cmd[1] == NULL){
		fprintf(stderr, "Command not find.\n");
		return -1; 
	}
	char* saveptr; 
	char* file_path = strtok_r(cmd[1], " ", &saveptr);
	if(file_path == NULL)
		return -1; 

    if(*direction == '>'){
        char** commands = splitCmd(cmd[0]);
 		fd = open(file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR);
 		if(fd < 0){
 			printf("Failed to open or create: %s\n", file_path);
 			return -1; 
 		}

    	dup2(fd, STDOUT_FILENO);
    	dup2(fd, STDOUT_FILENO);
    	close(fd); 
    	execvp(commands[0], commands); 
    	free(commands); 
    }
    else{
    	char** commands = splitCmd(cmd[0]);
    	fd = open(file_path, O_RDONLY); 
    	if(fd < 0){
    		fprintf(stderr, "Failed to open: %s: No such file or directory\n", file_path);
 			return -1; 
 		}
    	dup2(fd, STDIN_FILENO);
    	close(fd);
    	execvp(commands[0], commands); 
    	free(commands); 
    }

    return 0;
}
