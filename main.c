#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include "signalHandler.h"
//sudo apt-get install libncurses5-dev to install.
#include <ncurses.h>

int main(){
 	
    
   // printf(ANSI_COLOR_BLACK_BACKGROUND"start...\n");
	//initscr(); 
	//init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//use_default_colors();
 	sigs_Init(); 
 	 char* cmd;
 	int parent_cmd = -1, isBck_process = 0; 
	pid_t cpid = 0;
	uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char* str_tmp = (char*) malloc(sizeof(char) * 500); 


    if (pw == NULL)
    {
    	//@TODO do something if failed to get the user 
    }

	while(1){
	
		fprintf(stdout, ANSI_COLOR_GREEN "%s@Shell:> " ANSI_COLOR_RESET, pw->pw_name );
	    cmd = readCmd();
	    isBck_process = 0;
	     if(cmd[strlen(cmd) - 1] == '&'){
        	isBck_process = 1; 
        } 
        
        isHistoryCmd(cmd); 
        

        if(cmd != NULL && cpid != 0){
        //	printf("ADDINF TO HST\n");
        	addHistory(cmd); 
        //	printf("DONE ADDING TO HIST\n"); 
        }
        
        strncpy(str_tmp,cmd, 499); 
       // str_tmp[498] = '\0' ; 
 		char** tmp = splitCmd(str_tmp);
        //printf("DOING PARENT CHECH\n");
        parent_cmd =  excBuiltInFunc(tmp); 
        /// printf("DONE PARENT CHECH\n");
        
        if(parent_cmd == EXIT_SHELL){
        	exit(EXIT_SUCCESS); 
        }

        int forking = isParentCmd(parent_cmd);
       // printf("THIS THING IF FORKING: %d\n", forking );

        if(forking == -1){
        	cpid = fork(); 

	    	if(cpid == -1){
	    		perror("fork error");
	    	}
	    	else if(cpid == 0){
	    			excecute(cmd, 0, 1);
	    		}
	    	else{

	    		if(isBck_process == 1){
	    			addBck_process(cpid); 
	    		}
	    		else{
	    			pause();
	    		}
	    	}  

        }
       
	    parent_cmd = -1; 
	    free(cmd);
	}

	return 0; 
}

