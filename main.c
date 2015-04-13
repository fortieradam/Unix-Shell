#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int cmd;
int builtin;
int code;
COMMAND comtab[MAXCMDS];
int pipe1[2];
int pipe2[2];
int pipeProcess = FALSE;
int currcmd;
MYALIAS aliastab[MAXALIAS];

void clearQuotedString() {
	int index;
	for(index = 0; index < 500; index++) {
		quotedString[index] = '\0';
	}
}

void clearAliasTab() {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		aliastab[index].used = FALSE;
		aliastab[index].name[0] = '\0';
		aliastab[index].str[0] = '\0';
	}
}


void clearArgsTab(char* args[]) {
	int index = 0;
	while(index < MAXARGS) {
		args[index] = NULL;
		index++;
	}
}

void clearComTab() {
	int index = 0;
	while(index < MAXCMDS) {
		comtab[index].name = NULL;
		comtab[index].hasPipe = 0;
		comtab[index].hasIRed = 0;
		comtab[index].hasORed = 0;
		comtab[index].hasAmpersand = 0;
		comtab[index].infd = 0;
		comtab[index].outfd = 0;
		comtab[index].numArgs = 0;
		clearArgsTab(comtab[index].args);
		index++;
	}
}


void zeroStringArray(char array) {
	int i = 0;
	while(i < 50) {
		if(array == 'p') {
			stringArray[i] = '\0';
		}
		else if(array == 'c') {
			cwd[i] = '\0';
		}
		i++;
	}
}

void shell_init() {
	currcmd = 0;
	cmd = 0;
	builtin = 0;
	code = 0;
	pipe(pipe1);
	pipe(pipe2);
	clearComTab();
	clearAliasTab();
}

int getCommand() {
	if(!pipeProcess){
		zeroStringArray('c');
		printf("\nUnix-Shell:%s User01$ ", getcwd(cwd, sizeof(cwd)));
	}
	builtin = yyparse();
	if(builtin != 4 && builtin != 0 && builtin != 2 && builtin != 1) {
		return OK;
	}
	else if (builtin == BYE) {
		return BYE;
	}
	else {
		return ERRORS;
	}
}

void zeroArray(char* array) {
	int index = 0;
	int temp = strlen(array);
	while(index < temp) {
		array[index] = '\0';
		index++;
	}
}

/*
	This finds the next directory in the PATH environment var. Copies to dir.

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
*/

void findComPath(char* foundPath, COMMAND command) {
	char* path = getenv("PATH");
	char dir[strlen(path)];
	char* pathPtr = path;
	int outerBool = TRUE;
	int index = 0;
	int* count = &index;
	index = 0;

	int temp = 0;

	while(outerBool) {

		zeroArray(dir);
		
		pathPtr = findNextDirPath(dir, pathPtr, count, strlen(path));

 		struct dirent **namelist;
		int numOfElementsInDir = scandir(dir, &namelist, NULL, NULL);

		if (numOfElementsInDir < 0){
			temp++;
			if(temp == 10) {
				printf("Invalid command\n");
				outerBool = FALSE;
			}
		}
		else if(isInDir(namelist, numOfElementsInDir, command.name)){
			outerBool = FALSE;
		}
	}

	strcat(foundPath, dir);
}


void makeCommandStatement(char* comAndArgs[], COMMAND command, char* comPath) {
	comAndArgs[0] = comPath;
	int index = 1;
	while(index <= command.numArgs) {
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
		return -1;
	}
	else if(pid > 0) {
		int status;
		waitpid(pid, &status, NULL);
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
		else{
			exit(1);
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

void printEnv(){
	int i = 0;
	while(environ[i]) {
  		printf("%s\n", environ[i++]);
	}
}

void setNewEnv() {
	char name[500];
	char variable[500];
	char* token;
	token = strtok(stringArray, " "); // token is now "setenv"
	token = strtok(NULL, " "); // token is now the setenv name
	
	strcpy(name, token);
	//printf("\tname = %s\n", token);
	
	token = strtok(NULL, " "); // token is now the setenv string
	strcpy(variable, token);
	//printf("\tstring = %s\n", variable);
	
	setenv(name, variable, 1);
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
					// do nothing
				}
				break;
		case 6:	// CD
				printf("IM HERE\n");
				if(chdir(getenv("HOME")) == -1) {
					printf("Error: could not cd home");
				}
				else {}
				break;
		case 7: //ALIASSTRING
				break;
		case 8:	//ALIAS
				break;
		case 9: //ALIASCOMMAND
				break;
		case 10://UNALIAS
				break;
		case 11://SETENV
				setNewEnv();
				break;
		case 12://PRINTENV
				printEnv();
				break;
		case 13://UNSETENV
				break;
		default:
				printf("Unrecognized command\n");
				break;
	}
}

void fdCommandInit() {
	int commandsLeft = TRUE;
	int commandNum = 0;
	while(commandsLeft) {
		if(commandNum%2 == 0) {
			comtab[commandNum].outfd = pipe1[1];
			comtab[commandNum].infd = pipe2[0];
		}
		else {
			comtab[commandNum].outfd = pipe2[1];
			comtab[commandNum].infd = pipe1[0];
		}
		commandNum += 1;
		if(comtab[commandNum].name == NULL) {
			commandsLeft = FALSE;
		}
	}
} 

void printFunction() {
	printf("tlkcm \n");
	int commandsLeft = TRUE;
	int commandNum = 0;
	while(commandsLeft) {
		commandNum += 1;
		if(comtab[commandNum].name == NULL) {
			commandsLeft = FALSE;
		}
	}
}

void testPipingFunction() {
	int ret;
	pipeProcess = TRUE;
	char* buffer;
	int wrapperPID = fork();
	int fork1;
	int fork2;

	int continuePiping = TRUE;

	char* lsTest[2];
	lsTest[0] = ".//bin/ls";
	lsTest[1] = NULL;
	char* grepTest[3];
	grepTest[0] = ".//usr/bin/head";
	grepTest[1] = "-3";
	grepTest[2] = NULL;

	int stdOut = dup(1);
	int stdIn = dup(0);

	if(wrapperPID < 0) {
		printf("Error creating piping wrapper process\n");
	}
	else if(wrapperPID == 0) {
		pipeProcess = TRUE;
		while(continuePiping) {
			fork1 = fork();
			if(fork1 < 0) {
				printf("Error in the creation of the first Fork.\n");
			}
			else if(fork1 == 0) {
				pipeProcess = TRUE;
				close(comtab[currcmd].infd); // close read-end
		 		dup2(comtab[currcmd].outfd, 1); // copy write-end over stdout
		 		close(comtab[currcmd].outfd); // close write-end
		 		runIt(comtab[currcmd]);
		 		exit(1);
			}

			fork2 = fork();
			if(fork2 < 0){
				printf("Error in the creation of the second Fork\n");
			}
			else if(fork2 == 0) {
				pipeProcess = TRUE;
				// handle connection between first and second child
				close(comtab[currcmd+1].outfd); // close write-end
				dup2(comtab[currcmd+1].infd, 0); // copy read-end over stdin
				close(comtab[currcmd+1].infd); // close read-end
				// handle connection between second child and parent
				close(comtab[currcmd+1].infd); // close read-end
				dup2(comtab[currcmd+1].outfd, 1); // copy write-end over stdout
				close(comtab[currcmd+1].outfd); // close write-end
				runIt(comtab[currcmd+1]);
				exit(1);
			}

			currcmd += 2;
			continuePiping = FALSE;
		}
		exit(1);
	}

	int status;
	waitpid(wrapperPID, &status, NULL);
	buffer[ret] = '\0';
	pipeProcess = FALSE;

	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
	dup2(stdOut, 1);
	dup2(stdIn, 0);
}

void redirectAndRun(int oldfd, int newfd) {
	int stdIOCopy = dup(newfd);       	// clone to a new descriptor

	if(dup2(oldfd, newfd) < 0) {		// change to file
		printf("Cannot redirect to file\n");
	}

	runIt(comtab[currcmd]);

	close(oldfd);

	if(dup2(stdIOCopy, newfd) < 0) {	// change back from the clone
		printf("Cannot redirect back to standard IO\n");
	}

	close(stdIOCopy);					// close the clone
}

void execute_it() {
	if(comtab[currcmd].hasPipe != TRUE && comtab[currcmd].hasIRed != TRUE && comtab[currcmd].hasORed != TRUE) {
		runIt(comtab[currcmd]);
	}

	else if(comtab[currcmd].hasPipe == TRUE) {
		fdCommandInit();
		testPipingFunction();
	}
	else if(comtab[currcmd].hasORed && !comtab[currcmd + 1].name) {
		printf("No file given for output\n");
		return;
	}
	else if(comtab[currcmd].hasIRed && !comtab[currcmd + 1].name) {
		printf("No file given for input\n");
		return;
	}
	else if(comtab[currcmd].hasORed == TRUE) {
		int file = open(comtab[currcmd + 1].name, O_TRUNC | O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
		redirectAndRun(file, 1);
	}
	else if(comtab[currcmd].hasIRed == TRUE) {
		int file = open(comtab[currcmd + 1].name, O_RDONLY);
		redirectAndRun(file, 0);
	}
	else if(comtab[currcmd].hasORed == TWO) {
		int file = open(comtab[currcmd + 1].name, O_APPEND | O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
		redirectAndRun(file, 1);
	}
	else if(comtab[currcmd].hasIRed == TWO) {
		int file = open(comtab[currcmd + 1].name, O_RDONLY);
		redirectAndRun(file, 0);
	}
}

void processCommand() {
	if(builtin !=0 && builtin != -1) {
		do_it();
		currcmd += 1;
	}
	else {
		int status;
		int pid = fork();
		if(pid < 0) {
			printf("Main fork Error\n");
		}
		else if(pid == 0) {
			execute_it();	
			exit(1); //signal and then kill
		}

		waitpid(pid, &status, NULL);
	}
}

int main() {
	shell_init();
	int continueReading = TRUE;

	while(TRUE) {
		if(!pipeProcess) {
			currcmd = 0;
			zeroStringArray('p');
			clearComTab();
			cmd = getCommand();
			switch (cmd) {
				case BYE:		exit(0);
								break;
				case ERRORS:	exit(0);
								break;
				case OK:		processCommand();
								break;
			}
		}
	}

	return 0;
}
