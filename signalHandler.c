#include "signalHandler.h"
#include "builtIn_Cmd.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>



//varial used for signal handling see man signal
static struct sigaction control_c; 
static struct sigaction control_z;
static struct sigaction children;


#define MAX_BCK_PROCESS 100

//array used to keek track of back ground process 
 static pid_t bck_pross_PIDS[MAX_BCK_PROCESS] ; 
 static int pP = -1; 




/***********************************************************
* This fonction is signal handler.
* you can an handler for each signal  but I like them in on 
* place.
*
* param: sig int 
* return: none
***********************************************************/
void shellSignalHandler(int sig){
    
	if(sig == SIGINT || sig == SIGTSTP) printf("\n");
	else if(sig == SIGCHLD) { 
		pid_t cpid = wait(NULL);
        deleteBck_process(cpid); 
	}
	
}


/***********************************************************
* This fonction set up all the signal handlers.
* SIGINT : for control c
* SIGTSTP; for control z
* SIGCHL: for signal sent by child process. 
* you can look for the whole list on wikipedia lol 
* param: none
* return: none
***********************************************************/
void sigs_Init(){
 	control_c.sa_handler = shellSignalHandler;
 	sigaction(SIGINT, &control_c, NULL);
 	control_z.sa_handler = shellSignalHandler;
 	sigaction(SIGTSTP, &control_z, NULL);
 	children.sa_handler = shellSignalHandler;
 	sigaction(SIGCHLD, &children, NULL);
}


//sime stuff here, get a pid and add it to the array.
int addBck_process(pid_t pid){
	pP++; 
	bck_pross_PIDS[pP] = pid ; 
	return pP; 
}

//deleting from the process array 
int  deleteBck_process(pid_t pid){
	int position = -1; 
	for(int i = 0; i <= pP; i++){
		if(bck_pross_PIDS[i] == pid){
			position = i; 
			printf(ANSI_COLOR_YELLOW "[ %d ] is done\n" ANSI_COLOR_RESET ,pid );
		}
		
	}

	if(position >= 0){
		for(int i = position; i < pP - 1; i++){
			bck_pross_PIDS[i] = bck_pross_PIDS[i+1]; 
		}

		pP--; 
	}

	return position; 
}

//printing all the process
void showBackProcesses(){
	printf(ANSI_COLOR_BLUE "--BACK GROUND TASK--\n" ANSI_COLOR_RESET);
	for(int i = 0; i <= pP; i++){
		printf( ANSI_COLOR_BLUE "[ %d ]\n" ANSI_COLOR_RESET, bck_pross_PIDS[i] );
	}
}