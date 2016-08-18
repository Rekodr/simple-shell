#include "commandReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CMD_LENGTH 500
#define subCmd_max 200
#define DELIMETERS " \r\n\t&"
#define REDICTION_DELIMITER "<>"





/***********************************************************
* This function read input from stdin.
* Basically this reads the command from the user.
*
* param: none
* return: return a pointer to the command str.
***********************************************************/

char* readCmd(){
    int cmd_length = MAX_CMD_LENGTH ; 
    int input, p = 0;
    char* buffer = (char*) malloc(sizeof(char) * cmd_length);
    
    if(buffer == NULL){
    	fprintf(stderr, "memory allocation failled.\n");
    	return NULL; 
    }
    
    while(1){
    	input = getchar();
    	if(input == EOF || input == '\n'){
    		//need \0 in c for strings
    		buffer[p] = '\0'; 
    		return buffer; 
    	}
    	else{
    		buffer[p] = input;
    		p++; 

    		if(p >= cmd_length){
    			cmd_length += MAX_CMD_LENGTH ;
    			buffer = realloc(buffer, cmd_length);
    			if(buffer == NULL){
    				fprintf(stderr, "memory allocation failled.\n");
    				return NULL;
    			}
    		}
    	}
    }
}


/***********************************************************
* This finction split a string into substrings based the the
* delimeter  DELIMETERS(define on top) of the code.
*
* param: cmd char*
* return: return a pointer to the command str.
***********************************************************/

//sorry for the printf I did not ease any IDE lol :D
char** splitCmd(char* cmd){

   //printf("Here is the cmd %s\n", cmd ); //
   int cmd_length = subCmd_max, p;
   char* token, *saveptr, *_str_;
   char** commands = NULL;
   
  
   if(commands == NULL){
     //  printf("ALLOCATION STUFF\n");
       commands = malloc(sizeof(char) * cmd_length); 
    //fprintf(stderr, "Allocation failled.\n");
   	//	exit(EXIT_FAILURE);
   }
   
   
   for(p = 0, _str_ = cmd ;  ; ++p, _str_ = NULL){
   	  token = strtok_r(_str_, DELIMETERS, &saveptr); //better user strtok_r than strtok (read man.)
   	  if(token == NULL)
   	 	  break; 

   	  if(p >= cmd_length){
   	 	    cmd_length += subCmd_max;
   	 	    commands = realloc(commands, cmd_length);

   	 	    if(commands == NULL){
   	 		       fprintf(stderr, "Allocation failed.\n");
   	 		       exit(EXIT_FAILURE);
   	 	    }
   	  }
   	 commands[p] = token ; 
   }
   //make the end of the array to null 
   //don't call me on that but they might better way of doing this.
   commands[p] = NULL;
   return commands;
} 

//basecally same as the function above but for different delimeters
//I could make the two function in one but did realize that later.
//In fact you could use a function pointer to do that.
char** splitCmd_redirection(char* cmd){
   int cmd_length = subCmd_max, p;
   char* token, *saveptr, *_str_;
   char** commands = (char**) malloc(sizeof(char) * cmd_length);
  
   if(commands == NULL){
      fprintf(stderr, "Allocation failled.\n");
      exit(EXIT_FAILURE);
   }
   
  
   for(p = 0, _str_ = cmd ;  ; p++, _str_ = NULL){
     token = strtok_r(_str_, REDICTION_DELIMITER, &saveptr);
     if(token == NULL)
      break; 

     if(p >= cmd_length){
      cmd_length += subCmd_max;
      commands = realloc(commands, cmd_length);

      if(commands == NULL){
        fprintf(stderr, "Allocation failed.\n");
        exit(EXIT_FAILURE);
      }
     }
     commands[p] = token ; 
   }
   commands[p] = NULL;
   return commands;
} 



/***********************************************************
* This finction check if the cmd string contains | character.
* This is will be use to check if the usr  want to pipse stuff.
*
* param: cmd char*
* return: int 
***********************************************************/
int isPiping(char* cmd){
	int cmd_len = strlen(cmd);
	for(int i =0 ; i < cmd_len ; i++){
		if(cmd[i] == '|'){
			return i;
		}
	}
	return 0;
}


// same as the function on top but this time for >  <
// characters.
int isRedirecting(char* cmd, char* direction){
  int cmd_len = strlen(cmd);
  for(int i = 0 ; i < cmd_len; i++){
    if(cmd[i] == '>'){
      *direction = '>';
      return i;
    }
    else if(cmd[i] == '<'){
      *direction = '<';
      return i;
    }
  }

  return 0; 
}
