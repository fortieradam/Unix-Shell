#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int cmd;
int builtin;
int code;
COMMAND comtab[MAXCMDS];
int currcmd;
int pipeProcess = FALSE;

void clearArgsTab(char* args[]) {
	//printf("Entered clearArgsTab() with PID: %d\n", getpid());
	int index = 0;
	while(index < MAXARGS) {
		args[index] = NULL;
		index++;
	}
	//printf("Exited clearArgsTab() with PID: %d\n", getpid());
}

void clearComTab() {
	//printf("Entered clearComTab() with PID: %d\n", getpid());
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
	//printf("Exited clearComTab() with PID: %d\n", getpid());
}


void zeroStringArray(char array) {
	//printf("Entered zeroStringArray() with PID: %d\n", getpid());
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
	//printf("Exited zeroStringArray() with PID: %d\n", getpid());
}

void shell_init() {
	//printf("Entered shell_init() with PID: %d\n", getpid());
	currcmd = 0;
	cmd = 0;
	builtin = 0;
	code = 0;
	clearComTab();
	//printf("Exited shell_init() with PID: %d\n", getpid());
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
	//printf("Entered getCommand() with PID: %d\n", getpid());
	if(!pipeProcess){
		zeroStringArray('c');
		printf("Curr Pid: %d\n", getpid());
		printf("Unix-Shell:%s User01$ ", getcwd(cwd, sizeof(cwd)));
	}
	builtin = yyparse();
	//printf("Exited getCommand() with PID: %d\n", getpid());
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
	//printf("Entered recover_from_errors() with PID: %d\n", getpid());
	//printf("Exited recover_from_errors() with PID: %d\n", getpid());
	// find out if error occurs in middle of command
	// that is the command still has a “tail”
	// in this case you have to recover by “eating”
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
	//printf("Entered zeroArray() with PID: %d\n", getpid());
	int index = 0;
	int temp = strlen(array);
	while(index < temp) {
		array[index] = NULL;
		index++;
	}
	//printf("Exited zeroArray() with PID: %d\n", getpid());
}

/*
	This finds the next directory in the PATH environment var. Copies to dir.

	return: pathPtr = how far you have gone in the PATH var. Place holder that must be caught and reused in the calling function.
*/

char* findNextDirPath(char* dir, char* pathPtr, int* count, int size) {
	//printf("Entered findNextDirPath() with PID: %d\n", getpid());
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
	//printf("Exited findNextDirPath() with PID: %d\n", getpid());
	return pathPtr;
}

/*
	Checks if the given command name is in the directory given.

	return: TRUE or FALSE
*/

int isInDir(struct dirent **namelist, int numOfElementsInDir, char* command) {
	//printf("Entered isInDir() with PID: %d\n", getpid());
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
	//printf("Exited isInDir() with PID: %d\n", getpid());
	return FALSE;
}


/*
	Find the path to the binary given in the command
	
	IMPORTANT: foundPath is the where the found path is stored. Use var passed in to access path
*/

void findComPath(char* foundPath, COMMAND command) {
	//printf("Entered findComPath() with PID: %d\n", getpid());
	char* path = getenv("PATH");
	char dir[strlen(path)];
	char* pathPtr = path;
	int outerBool = TRUE;
	int index = 0;
	int* count = &index;
	index = 0;
	int found;

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
		else if(found = isInDir(namelist, numOfElementsInDir, command.name)){
			outerBool = FALSE;
		}
	}

	strcat(foundPath, dir);
	//printf("Exited findComPath() with PID: %d\n", getpid());
}


void makeCommandStatement(char* comAndArgs[], COMMAND command, char* comPath) {
	//use command to get "PATH + command" + "arg1" + "arg2" + "etc" and ret the double ptr
	//printf("Entered makeCommandStatement() with PID: %d\n", getpid());
	comAndArgs[0] = comPath;
	int index = 1;
	while(index <= command.numArgs) {
		comAndArgs[index] = command.args[index - 1];
		index++;
	}
	comAndArgs[index] = NULL;
	//printf("Exited makeCommandStatement() with PID: %d\n", getpid());
}

/*
	Calls necessary functions to find the binary and execute it based on the command structure given. Handles forking and such.

	return: TRUE or FALSE
*/

int runIt(COMMAND command) {
	//printf("Entered runIt()) with PID: %d\n", getpid());
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
		currcmd += 1;
		if( status == -1) {
			exit(status);
		}
	}
	//printf("Exited runIt() with PID: %d\n", getpid());
	return 0;
}

void displacedStringArray(int j) {
	//printf("Entered displacedStringArray() with PID: %d\n", getpid());
	int i = 0;
	while(stringArray[i] != 0 && j != MAXPATH) {
		stringArray[i] = stringArray[j];
		i++;
		j++;
	}
	//printf("Exited displacedStringArray() with PID: %d\n", getpid());
}

void do_it() {
//	printf("Entered do_it() with PID: %d\n", getpid());
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
	//printf("Exited do_it() with PID: %d\n", getpid());
}



void executePipe() {
/*
	printf("HI IM HERE FIRST\n");
	int fd[2];
	pipe(fd);
	int stdout = dup(1);
	pipeProcess = TRUE;
	pid_t pid;
	pid_t parent = getpid();
	pid = fork();//Forking process
	printf("Forking process: %d\n", pid);
	*/
	//printf("Entered executePipe() with PID: %d\n", getpid());
	int stdOut = dup(1);
	int stdIn = dup(0);
	int pid;
	char* buffer;
	int pipe1[2];
	int pipe2[2];
	int ret;
	char* lsTest[2];
	lsTest[0] = ".//bin/ls";
	lsTest[1] = NULL;
	char* grepTest[3];
	grepTest[0] = ".//usr/bin/grep";
	grepTest[1] = "h";
	grepTest[2] = NULL;

	pipe(pipe1);

	pid = fork(); //first child

	if(pid < 0) {
		printf("Code Line 314 Errorrrr\n");
	}
	else if(pid == 0) {
		pipeProcess = TRUE;
		close(pipe1[0]); // close read-end
 		dup2(pipe1[1], 1); // copy write-end over stdout
 		close(pipe1[1]); // close write-end
 		//runIt(comtab[currcmd]);
 		execve("/bin/ls", lsTest, NULL);
 		exit(1);
	}

	pipe(pipe2);

	pid = fork(); //second child

	if (pid < 0) {
		printf("Fucking Error on Second Child Fork\n");
	}

	if (pid == 0) {
		// handle connection between first and second child
		pipeProcess = TRUE;
		close(pipe1[1]); // close write-end
		dup2(pipe1[0], 0); // copy read-end over stdin
		close(pipe1[0]); // close read-end
		// handle connection between second child and parent
		close(pipe2[0]); // close read-end
		dup2(pipe2[1], 1); // copy write-end over stdout
		close(pipe2[1]); // close write-end
		//runIt(comtab[currcmd]);
		execve("/usr/bin/grep", grepTest, NULL);
		exit(1);
	}

	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
	dup2(stdOut, 1);
	dup2(stdIn, 0);

	ret = read(pipe2[0], buffer, 128);
	if (ret <= 0) {
	 printf("Reading problem!\n");
	}

	buffer[ret] = '\0';
	printf("%s", buffer);
	//printf("Exited executePipe() with PID: %d\n", getpid());
	/*
	else if(pid > 0) {
		/*
		int status;
		waitpid(pid ,&status, 0);
		printf("WAITING DONE\n");
		pipeProcess = FALSE;          // DOES NOT WORK !!!! ONLY FOR ONE PIPE!
		dup2(fd[0], 0);
		char* grepTest[2];
		grepTest[0] = ".//usr/bin/sort";
		grepTest[1] = NULL;
		int ret = read(fd[0], buffer, 128);
		buffer[ret] = NULL;
		printf("Buffer: %s\n", buffer);
		//printf("Before execution\n");
		//execve("/usr/bin/sort", grepTest, NULL);
		//printf("After execution\n");
		/*
		if(dup2(stdout, 1) >= 0){
			runIt(comtab[currcmd]);
		}
	}
	else {
		/*
		printf("CHILD HERE\n");
		char* test[2];
		test[0] = ".//bin/ls";
		test[1] = NULL; 
		dup2(fd[1], 1);
		execve("/bin/ls", test, NULL);
		/*
		runIt(comtab[currcmd]);
		dup2(fd[1], 1);
		exit(0); // exits program... problem
		/*
		runIt(comtab[currcmd]);
		_exit(EXIT_FAILURE);


	}
	*/

}

void execute_it() {
	//printf("Entered execute_it() with PID: %d\n", getpid());
	if(comtab[currcmd].hasPipe != TRUE && comtab[currcmd].hasIRed != TRUE && comtab[currcmd].hasORed != TRUE) {
		printf("GOT ERE\n");
		runIt(comtab[currcmd]);
	}

	if(comtab[currcmd].hasPipe == TRUE) {
		printf("GOT HERE\n");
		executePipe();
	}
	//printf("Exited execute_it() with PID: %d\n", getpid());

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
	//printf("Entered processCommand() with PID: %d\n", getpid());
	if(builtin !=0 && builtin != -1) {
		do_it();
	}
	else {
		execute_it();
	}
	//printf("Exited processCommand() with PID: %d\n", getpid());
}

int main() {
	//printf("Entered main() with PID: %d\n", getpid());
	//if(!pipeProcess) {
		shell_init();
		//comtab[0].hasPipe = TRUE;
		//clearComTab();
		/*
		comtab[0].name = "ls";
		comtab[0].hasPipe = TRUE;
		comtab[0].hasIRed = FALSE;
		comtab[0].hasORed = FALSE;
		comtab[0].infd = 0;
		comtab[0].outfd = 0;
		comtab[0].numArgs = 0;
		clearArgsTab(comtab[0].args);

		comtab[1].name = "grep";
		comtab[1].hasPipe = FALSE;
		comtab[1].hasIRed = FALSE;
		comtab[1].hasORed = FALSE;
		comtab[1].infd = 0;
		comtab[1].outfd = 0;
		comtab[1].numArgs = 1;
		clearArgsTab(comtab[1].args);
		comtab[1].args[0] = "h";
		*/
		//execute_it();
	//}
		while(TRUE) {
			int status;
			int pid = fork();
			if(pid < 0) {
				printf("Main fork Error\n");
			}
			else if(pid == 0) {
				//printPrompt();
				if(!pipeProcess) {
					//printf("Proocess ID: %d\n", getpid());
					zeroStringArray('p');
					clearComTab();
					printf("What the Fuck am I here for!?\n");
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
				exit(1);//signal and then kill
			}

			waitpid(pid, &status, NULL);//Parent wait and reloop to fork
		}

	return 0;
}
