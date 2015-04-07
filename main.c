#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int cmd = 0;
int builtin = 0;
int code = 0;
COMMAND comtab[MAXCMDS];

/*
COMMAND initializeCom(char* name, int hasPipe, int hasIRed, int hasORed, int hasAmpersand, int infd, int outfd, int numArgs, char** args){
	COMMAND command;
	command.name = name;
	command.hasPipe = hasPipe;
	command.hasIRed = hasIRed;
	command.hasORed = hasORed;
	command.hasAmpersand = hasAmpersand;
	command.infd = infd;
	command.outfd = outfd;
	command.numArgs = numArgs;
	command.args = args;
	return command;
}
*/

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
	printf("builtin: %d\n", builtin);
	if(builtin != 4 && builtin != 0 && builtin != 2) {
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
	// that is the command still has a tail
	// in this case you have to recover by eating
	// the rest of the command
	// to do this: use yylex() directly
}
/*
void list(char* dir) {
	struct dirent **namelist;
	int n = scandir(dir, &namelist, NULL, NULL);
	if (n < 0) {
		printf("No such file or directory\n", dir);
	}
	else {
		int i;
		for(i = 0; i < n; i++) {
			if(namelist[i]->d_name[0] != '.') { // ignore hidden files
				printf("%s\n", namelist[i]->d_name);
			}
			free(namelist[i]);
		}
		free(namelist);
	}
}
*/
void zeroArray(char* array) {
	int index = 0;
	int temp = strlen(array);
	while(index < temp) {
		array[index] = NULL;
		index++;
	}
}

/*
	This finds the next directory in the PATH environment var.

	return: pathPtr = how far you have gone in the PATH var. Place holder that must be caught and reused in the calling function.
*/

char* findNextDirPath(char* dir, char* pathPtr, int* count, int size) {
	int innerBool = TRUE;
	int index = 0;
	while(innerBool) {
		if(*count < size && *pathPtr != ':') {
			dir[index] = *pathPtr;
			pathPtr++;
			index++;
			(*count)++;
		}
		else if(index >= size) {
			innerBool = FALSE;
		}
		else {
			innerBool = FALSE;
			pathPtr += 1;
			return pathPtr;
		}
	}
	return pathPtr;
}

/*
	Checks if the given command name is in the directory given.

	return: TRUE or FALSE
*/

int isInDir(struct dirent **namelist, int numOfElementsInDir, char* command) {
	int i;
		for(i = 0; i < numOfElementsInDir; i++) {
			if(namelist[i]->d_name[0] != '.') { // ignore hidden files
				if(strcmp(namelist[i]->d_name, command) == 0){
					return TRUE;
				}
			}
		free(namelist[i]);
	}

	free(namelist);
	return FALSE;
}


/*
	Find the path to the binary given in the command
	
	IMPORTANT: foundPath is the where the found path is stored. Use var passed in to access path
*/

void findComPath(char* foundPath, COMMAND command) {
	char* path = getenv("PATH");
	char dir[strlen(path)];
	char* pathPtr = path;
	int outerBool = TRUE;
	int index = 0;
	int* count = &index;
	index = 0;
	int found;

	while(outerBool) {

		zeroArray(dir);
		
		pathPtr = findNextDirPath(dir, pathPtr, count, strlen(path));

 		struct dirent **namelist;
		int numOfElementsInDir = scandir(dir, &namelist, NULL, NULL);

		if (numOfElementsInDir < 0) {
			printf("No such file or directory\n");
		}
		else if(found = isInDir(namelist, numOfElementsInDir, command.name)){
			//printf("Found %s in: %s\n", command.name,dir);
			outerBool = FALSE;
		}
	}

	strcat(foundPath, dir);
}


void makeCommandStatement(char* comAndArgs[], COMMAND command, char* comPath) {
	//use command to get "PATH + command" + "arg1" + "arg2" + "etc" and ret the double ptr
	comAndArgs[0] = comPath;
	int index = 1;
	while(index <= command.numArgs) {
		printf("HEREEEE\n");
		comAndArgs[index] = command.args[index - 1];
		index++;
	}
	comAndArgs[index] = NULL;

}

/*
	Calls necessary functions to find the binary and execute it based on the command structure given. Handles forking and such.

	return: TRUE or FALSE
*/

int runIt(COMMAND command) {

	int pid = fork();
	if(pid == -1) {
		printf("Got Here\n");
		return -1;
	}
	else if(pid > 0) {
		int status;
		wait(&status);
	}
	else {
		char comPath[strlen(getenv("PATH"))];
		char* comAndArgs[MAXARGS];
		zeroArray(comPath);
		findComPath(comPath, command);
		strcat(comPath, "/");
		strcat(comPath, command.name);

		char executeBinaryStatement[sizeof(comPath)+2];
		strcpy(executeBinaryStatement, "./");
		strcat(executeBinaryStatement, comPath);
		makeCommandStatement(comAndArgs, command, executeBinaryStatement);
		int status = execve(comPath, comAndArgs, NULL);
		if( status == -1) {
			exit(status);
		}
	}
	return 0;
}

void displacedStringArray(int j) {
	int i = 0;
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
		default:
				printf("unrecognized command\n");
				break;
	}
	/*
	switch (builtin) {
		case CDHome  //gohome();
		case CDPath  //chdir(path);
		case ALIAS 
		case UNALIAS 
		case SETENV 
		case PRINTENV 
		
	}
	*/
}


void execute_it() {
	if(comtab[0].hasPipe != 1 && comtab[0].hasIRed != 1 && comtab[0].hasORed != 1) {
		runIt(comtab[0]);
	}
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
	if(builtin !=0 && builtin != -1) {
		do_it();
	}
	else {
		execute_it();
	}
}

int main() {
	shell_init();
	COMMAND command;
	command.name = "ls";
	command.hasPipe = 0;
	command.hasIRed = 0;
	command.hasORed = 0;
	command.hasAmpersand = 0;
	command.infd = 0;
	command.outfd = 0;
	command.numArgs = 1;
	char** ptr2 = NULL;
	command.args[0] = "-alh";
	char* ptr = NULL;
	runIt(command);
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
