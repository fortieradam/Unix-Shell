#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cmd = 0;
int builtin = 0;
//char* path = "";
int code = 0;

void shell_init() {
	// init all variables
	// define (allocate storage) for some var/tables
	// init all tables (e.g., alias table)
	// get PATH environment variable (use getenv())
	// get HOME env variable (also use getenv())
	// disable anything that can kill your shell.
	// (the shell should never die; only can be exit)
	// do anything you feel should be done as init
}

int getCommand() {
	//init_scanner_and_parser();
	printf("getCommand\n");
	builtin = yyparse();
	if(builtin) {
		return OK;
	}
	else {
		//understand_errors();
		printf("ERRORrrr\n");
		return ERRORS; // may need to update
	}
}

void recover_from_errors() {
	// find out if error occurs in middle of command
	// that is the command still has a “tail”
	// in this case you have to recover by “eating”
	// the rest of the command
	// to do this: use yylex() directly
}

void do_it() {
	switch(builtin) {
		case 1:	if(chdir(path) == -1) {
					printf("Error: could not cd\n");
				}
				else {
					printf("successful cd\n");
				}
				break;
		case 2: 
				if(getcwd(path, sizeof(path)) != NULL) {
				printf("%s\n", path);
				}
				else {
					printf("An error has occurred\n");
				}
				break;
		default:
				printf("unrecognizd command\n");
				break;
	}
	/*
	switch (builtin) {
		case CDHome … //gohome();
		case CDPath … //chdir(path);
		case ALIAS …
		case UNALIAS …
		case SETENV …
		case PRINTENV …
		…
	}
	*/
}


void execute_it() {
	/*
	// handle command execution, pipelining, i/o redirection, and background processing
	// utilize a command table whose components are plugged in during parsing by yacc

	// check command accessibility and executability
	if(!executable()) {
		// use access() system call with X_OK
		printf("Command not found");
		return;
	}
	/*
	// check io file existence in case of io-redirection
	if(check_in_file() == SYSERR) {
		printf("Can't read from: %s", srcf);
		return;
	}
	if(check_out_file() == SYSERR) {
		printf("Can't write to: %s", distf);
		return;
	}

	// build up the pipeline (create and set up pipe end points (using pipe, dup)
	// process background
	*/
}

void processCommand() {
	if(builtin) {
		do_it();
	}
	else {
		execute_it();
	}
}

void zeroStringArray() {
	int i = 0;
	while(i < 50) {
		path[i] = NULL;
		i++;
	}
}

int main() {
	//shell_init();
	while(TRUE) {
		//printPrompt();
		zeroStringArray();
		cmd = getCommand();
		switch (cmd) {
			case BYE:		exit(0);
							break;
			case ERRORS:	recover_from_errors();
							break;
			case OK:		processCommand();
							break;
		}
	}
	return 0;
	/*
	While (1) {
		printPrompt();
		Switch (CMD = getCommand()) {
		Case: BYE exit();
		Case: ERRORS recover_from_errors();
		Case: OK processCommand();
		}
	}
	*/
}
