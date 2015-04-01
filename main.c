#include "shell.h"

int cmd = 0;
int builtin = 0;
char* path = "";

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
	init_scanner_and_parser();
	if(yyparse()) {
		understand_errors();
		return ERRORS; // may need to update
	}
	else {
		return OK;
	}
}

void recover_from_errors() {
	// find out if error occurs in middle of command
	// that is the command still has a “tail”
	// in this case you have to recover by “eating”
	// the rest of the command
	// to do this: use yylex() directly
}

void processCommand() {
	if(builtin) {
		do_it();
	}
	else {
		execute();
	}
}

void do_it() {
	switch(builtin) {
		case 3:	gohome(); 		// CDHome
				break;
		case 4:	chdir(path);	// CDPath
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
	// handle command execution, pipelining, i/o redirection, and background processing
	// utilize a command table whose components are plugged in during parsing by yacc

	// check command accessibility and executability
	if(!executable()) {
		// use access() system call with X_OK
		printf("Command not found");
		return;
	}

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
}

void main() {
	shell_init();
	while(TRUE) {
		printPrompt();
		cmd = getCommand();
		switch (cmd) {
			case BYE:		exit();
							break;
			case ERRORS:	recover_from_errors();
							break;
			case OK:		processCommand();
							break;
		}
	}
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
