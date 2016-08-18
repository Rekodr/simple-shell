#ifndef _SIGNAL_HANDLER_h
#define _SIGNAL_HANDLER_h

#include <sys/types.h>

void shellSignalHandler(int sig); 
void sigs_Init(); 
int addBck_process(pid_t pid);
int  deleteBck_process(pid_t pid);
void showBackProcesses(); 

#endif