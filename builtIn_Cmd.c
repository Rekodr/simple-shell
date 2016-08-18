#include "builtIn_Cmd.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include <fcntl.h>


#define MAX_HISTORY_SIZE 100

static int histSize = MAX_HISTORY_SIZE ;
static char** cmdHistoryList; 
static int  hist_pos = 0; 



/***********************************************************
* This array list all the built in command for this shell.
* whenever you add a new internal cmd dont forget to add it 
* here. the order have to the as in the array of functions 
* below.

***********************************************************/
char* internalFunctionList[] = {
	"cd",
	"pwd",
	"exit",
	"history",
	"jobs"
};


/***********************************************************
* This is an array fonction pointer, point to the different 
* built in command implemented function. (cool stuff)
* 
* this function all can an array of str. you can change that
* as you wish
***********************************************************/
int (*internalFunction[]) (char**) = {
	&cd,
	&pwd,
	&exitShell,
	&getHistory_cmd,
	&jobs,
};


/***********************************************************
* This fonction change the process directory.
* Note that if you want to change the parent process dir,
* this should be run by the parent process him self. 
*
* param: cmd char**
* return: define ( see shell.h)
***********************************************************/
int cd(char** cmd){
	if(cmd[1] == NULL){
		fprintf(stderr, "program cd expects argument\n" );
		return PARENT_CMD_FAIL; 
	}
    int success = chdir(cmd[1]); 
    if(success == -1){
    	fprintf(stderr, "ERROR: %s\n", strerror(errno));
    	return PARENT_CMD; 
    }
    return PARENT_CMD_FAIL; 
}


/***********************************************************
* This fonction return the pwd of the current process.
*
* param: cmd char**
* return: define ( see shell.h)
***********************************************************/
int pwd(char** cmd){
	int path_size = 1000; 
	char* path = (char*) malloc(sizeof(char) * path_size);
	path = getcwd(path, path_size);
    
    int fd = -1; 
    int saved_FILENO; 
	for(int i = 0 ; cmd[i] != NULL; i++){

		//in case the user want to redict the output
		if(strncmp(cmd[i], ">", 1) == 0){

			char* saveptr; 	
            char* file_path = strtok_r(cmd[1+i], " ", &saveptr);

            //fd is a filedescriptor. Here open a file with 
            // reading, writing and excecutin permition. see man open()
			fd = open(file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR) ; 
   
			if(fd < 0){
				fprintf(stderr, "Failed to open or create: %s\n", cmd[1+i]);
			}

			//need to save the current file descritor so tha you can
			//restor stdout to the screen here. 
			saved_FILENO = dup(STDOUT_FILENO);

			//duplacation Of STDOUT_FILENO
			//man page for dup2();
			dup2(fd, STDOUT_FILENO); 
			close(fd);
			if(path != NULL){
				//remember that fs point to stdout now :p right ? 
				fprintf(stdout, "%s\n", path);
				free(path); 
				path = NULL ; 
			}  
			break;  
		}
	}

//here is the important step, need to restore to the default STDOUT_FILENO
//remember that we saved the default one in saved_FILENO right.
    if(fd != -1){
			dup2(saved_FILENO, STDOUT_FILENO); 
			close(saved_FILENO);   
	}

	if(path != NULL){
		fprintf(stdout, "%s\n", path);
		free(path);
		return PARENT_CMD; 
	}
	return PARENT_CMD_FAIL; 
}


//exiting the shell.
int exitShell(char** cmd){
	printf("ADIOS ><\n");
	for(int i = 0; i < hist_pos; i++){
		free(cmdHistoryList[i]); 
	}
	free(cmdHistoryList); 
	
	return EXIT_SHELL; 
}

//variable to keep track of history
//static int  hist_pos = 0; 

//quite straight forwart, leave questions on my git if needed
int addHistory(char* cmd){

	if(cmdHistoryList == NULL){
		cmdHistoryList = (char**) malloc(sizeof(char) * MAX_HISTORY_SIZE); 

		//for(int i = 0; i < MAX_HISTORY_SIZE ; i++){
		//	cmdHistoryList[i] = NULL; 
		//}

	}

	if(cmd != NULL && strlen(cmd) > 0 ){
         
        //printf("ALLOCATION STUFF GOING ON\n");
        cmdHistoryList[hist_pos] =  malloc(sizeof(char) * (strlen(cmd) + 1));

        if(cmdHistoryList[hist_pos] != NULL){

        	strncpy(cmdHistoryList[hist_pos], cmd, strlen(cmd));
        	//printf("DONE COPYING TO HISTORY\n"); 

        	cmdHistoryList[hist_pos][strlen(cmd)] = '\0'; 
        	hist_pos++;
        	if(hist_pos >= MAX_HISTORY_SIZE){
        		//printf("REALLOCATION OF THE HISTORY TABLE\n");
        		histSize += MAX_HISTORY_SIZE; 
        		cmdHistoryList = realloc(cmdHistoryList, histSize); 
        	}

        	cmdHistoryList[hist_pos] = NULL;
        	//printf("DONE ADDING TO HISTORY\n");
        }
    }

return PARENT_CMD ;
}


int getHistory_cmd(char** cmd){

    
	for(int i = 0; hist_pos > 0 && cmdHistoryList[i] != NULL ; i++){
		printf("[ %d ] %s\n", i, cmdHistoryList[i]);
	}
	return PARENT_CMD; 
}

int jobs(){
	showBackProcesses(); 
	return PARENT_CMD ;
}

int excBuiltInFunc(char** cmd){
	int n = sizeof(internalFunctionList) / sizeof(char*);
    
    if(cmd[0] == NULL){
    	return PARENT_CMD_FAIL; 
    }
	for(int i = 0; i < n; i++){
       if(strcmp(internalFunctionList[i], cmd[0]) == 0){
       		int success = (*internalFunction[i])(cmd) ; 
       		return success; 
       }
	}
	return -1; 
}

int isParentCmd(int status){
	if(status == PARENT_CMD || status == EXIT_SHELL || status == PARENT_CMD_FAIL)
		return 0; 

	else
		return - 1; 
}


int  isHistoryCmd(char* cmd){
     
    if(cmd == NULL || cmd[0] != '!')
        return -1; 
     
    char c ; 
    int index; 
    for(int i = 1; i < strlen(cmd); i++){
        if(cmd[i] < '0' || cmd[i] > '9'){
        	printf("Invalid command.\n");
            return PARENT_CMD_FAIL;
        }
    }
    if(sscanf(cmd,"%c%d",&c, &index) == 2){
        
        if( index >= 0 && index < hist_pos){

        	int sizeL = strlen(cmdHistoryList[index]); 
            cmd = realloc(cmd , sizeof(char*) * sizeL ); 
           // printf("This is the cmd to run: %s\n", cmdHistoryList[index]);
            strncpy(cmd, cmdHistoryList[index], strlen(cmdHistoryList[index])); 
            cmd[strlen(cmdHistoryList[index])] = '\0';
            return PARENT_CMD; 

        }   
    }
    return PARENT_CMD_FAIL; 
     
}
