CC = gcc
CFLAGS = --std=gnu99  -lm -g -Wall -Werror -O  
Headers = commandReader.h shell.h signalHandler.h builtIn_Cmd.h
Objects = commandReader.o main.o shell.o signalHandler.o builtIn_Cmd.o


%.o : %.c $(Headers)
	$(CC) $(CFLAGS) -c $< -o $@

Shell: $(Objects)
	$(CC) $(CFLAGS) $^ -o $@

PHONY: clean

clean:
	rm -f $(Objects)	