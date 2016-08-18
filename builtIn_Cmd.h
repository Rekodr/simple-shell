#ifndef _BUILT_IN_CMD
#define _BUILT_IN_CMD

#include "signalHandler.h"

#define PARENT_CMD 54
#define EXIT_SHELL 100
#define PARENT_CMD_FAIL -54


//thanks to Andrejs on stackoverflow this.
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*
 * 
 * all function under this text should be buildin function declation
 *
 *
 */
int excBuiltInFunc(char**) ;
int isParentCmd(int status); 
int cd(char** cmd);
int pwd();
int exitShell(); 
int addHistory(); 
int getHistory_cmd();
int jobs();
int  isHistoryCmd(char* cmd); 
#endif
