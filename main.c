#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int cmd = 0;
int builtin = 0;
//char* path = "";
int code = 0;

void zeroStringArray(char array) {
	int i = 0;
	while(i < 50) {
		if(array == 'p') {
			stringArray[i] = NULL;
		}
		else {
			cwd[i] = NULL;
		}
		i++;
	}
}

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
	zeroStringArray('c');
	printf("Unix-Shell:%s User01$ ", getcwd(cwd, sizeof(cwd)));
	builtin = yyparse();
	if(builtin != 4 && builtin != 0) {
		return OK;
	}
	else if (builtin == BYE) {
		return BYE;
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

void list() {
	struct dirent **namelist;
	int n = scandir(".", &namelist, NULL, NULL);
	if (n < 0) {
		printf("scandir error has occurred\n");
	}
	else {
		int i;
		for(i = 0; i < n; i++) {
			printf("%s\n", namelist[i]->d_name);
			free(namelist[i]);
		}
		free(namelist);
	}
}

void displacedStringArray(int j) {
	int i = 0;
	printf("stringArray length: %d\n", strlen(stringArray));
	while(stringArray[i] != 0 && j != MAXPATH) {
		stringArray[i] = stringArray[j];
		i++;
		j++;
	}
}

void do_it() {
	switch(builtin) {
		case 1:
				printf("scan error\n");
				break;
		case 5:	// CDSTRING
				displacedStringArray(3);
				if(chdir(stringArray) == -1) {
					printf("Error: could not cd\n");
				}
				else {
					//zeroStringArray(c);
					//getcwd(cwd, sizeof(cwd))
				}
				break;
		case 6:	// CD
				if(chdir(getenv("HOME")) == -1) {
					printf("Error: could not cd home");
				}
				else {}
				break;
		case 7:	// ECHO
				displacedStringArray(5);
				if(stringArray[0] != 0) {
					printf("%s\n", stringArray);
				}
				else {
					printf("could not echo\n");
				}
				break;
		case 8: // MKDIR
				if(stringArray[0] != 0) {
					mkdir(stringArray, ACCESSPERMS);
				}
				else {
					printf("no string to mkdir\n");
				}
				break;
		case 9: // PWD
				if(getcwd(cwd, sizeof(cwd)) != NULL) {
				printf("%s\n", cwd);
				}
				else {
					printf("An error has occurred\n");
				}
				break;
		case 10: // LS
				list();
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

int main() {
	//shell_init();
	while(TRUE) {
		//printPrompt();
		zeroStringArray('p');
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
