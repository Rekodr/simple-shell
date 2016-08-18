#ifndef _SHELL_H_
#define _SHELL_H_

#include "commandReader.h"
#include "builtIn_Cmd.h"


void excecute(char* cmd, int parent, int child);
int redirect(char** cmd, char* direction); 


#endif