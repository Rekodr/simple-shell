#ifndef _COMMMAND_READER_H
#define _COMMMAND_READER_H

char* readCmd();
char** splitCmd(char* cmd); 
char** splitCmd_redirection(char* cmd); 
int isPiping(char* cmd); 
int isRedirecting(char* cmd, char* direction); 

#endif 