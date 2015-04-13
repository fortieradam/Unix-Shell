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
		aliastab[index].name = NULL;
		aliastab[index].str = NULL;
	}
}


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
			stringArray[i] = '\0';
		}
		else if(array == 'c') {
			cwd[i] = '\0';
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
	pipe(pipe1);
	pipe(pipe2);
	clearComTab();
	//printf("Exited shell_init() with PID: %d\n", getpid());
	clearAliasTab();
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
		//printf("Curr Pid: %d\n", getpid());
		printf("Unix-Shell:%s User01$ ", getcwd(cwd, sizeof(cwd)));
	}
	builtin = yyparse();
	printf("builtin: %d\n", builtin);
	//printf("Exited getCommand() with PID: %d\n", getpid());
	if(builtin != 4 && builtin != 0 && builtin != 2 && builtin != 1) {
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
		array[index] = '\0';
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
	printf("\tname = %s\n", token);
	
	token = strtok(NULL, " "); // token is now the setenv string
	strcpy(variable, token);
	printf("\tstring = %s\n", variable);
	
	//setenv(name, variable, 1);
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
				printf("IM HERE\n");
				if(chdir(getenv("HOME")) == -1) {
					printf("Error: could not cd home");
				}
				else {}
				break;
		case 7: //ALIASSTRING
				//printf("found alias string!\n");
				break;
		case 8:	//ALIAS
				//printf("found alias!\n");
				break;
		case 9: //ALIASCOMMAND
				//printf("found alias command!\n");
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
				printf("unrecognized command\n");
				break;
	}
	//printf("Exited do_it() with PID: %d\n", getpid());
}

/*
	Create a processing function to abstract the execution of the pipe... Allows for the recursive calling of pipe and the proper handling pf redirection.
	Command | Comand | Comand < file > file2  < --- Lastly it will be written here.
								  ^
								  |
						This need to be given to the first
						Command

			Pass to yacc via file or expand alias in yacc
			forking to background... dont wait for them to finish but ensure they do


			glob -- wildcard matching
*/

/*
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
	was comment end
	//printf("Entered executePipe() with PID: %d\n", getpid());
	pipeProcess = TRUE;
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
	grepTest[0] = ".//usr/bin/head";
	grepTest[1] = "-3";
	grepTest[2] = NULL;

	pipe(pipe1);

	pid = fork(); //first child
	printf("ProID: %d\n", pid);

	if(pid < 0) {
		printf("Code Line 314 Errorrrr\n");
	}
	else if(pid == 0) {
		pipeProcess = TRUE;
		close(pipe1[0]); // close read-end
 		dup2(pipe1[1], 1); // copy write-end over stdout
 		close(pipe1[1]); // close write-end whaaaaaaaaaaaaaaaa
 		//printf("First Command: %s\n", comtab[currcmd].name);
 		//runIt(comtab[currcmd]);
 		//currcmd += 1;
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
		close(pipe2[1]); // close write-end whaaaaaaaa
		//printf("CurrentCmd: %d\n", currcmd);
		//runIt(comtab[currcmd]);
		currcmd += 1;
		execve("/usr/bin/head", grepTest, NULL);
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
	pipeProcess = FALSE;
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
	was comment end

}
*/

void fdCommandInit() {
	//printf("fdCommandInit\n");
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
		printf("Command %d infd %d outfd %d\n", commandNum, comtab[commandNum].infd, comtab[commandNum].outfd);
		commandNum += 1;
		if(comtab[commandNum].name == NULL) {
			commandsLeft = FALSE;
		}
	}
}

void testPipingFunction() {
	int ret; // NEED
	pipeProcess = TRUE;
	//printf("testPipingFunction\n");
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
		 		close(comtab[currcmd].outfd); // close write-end whaaaaaaaaaaaaaaaa
		 		//printf("First Command: %s\n", comtab[currcmd].name);
		 		runIt(comtab[currcmd]);
		 		//currcmd += 1;
		 		//execve("/bin/ls", lsTest, NULL);
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
				close(comtab[currcmd+1].outfd); // close write-end whaaaaaaaa
				//printf("CurrentCmd: %d\n", currcmd);
				runIt(comtab[currcmd+1]);
				//execve("/usr/bin/head", grepTest, NULL);
				exit(1);
			}

			currcmd += 2;
			continuePiping = FALSE;
		}
		//printf("wrapper: %d\n", getpid());

		//ret = read(pipe2[0], buffer, 128);
		exit(1);
	}

	int status;
	//printf("before wait\n");
	waitpid(wrapperPID, &status, NULL);
	//printf("after wait\n");
	
	//ret = read(pipe2[0], buffer, 128);
	//printf("BullShit\n");
	buffer[ret] = '\0'; // NEED
	//printf("End of piping function\n");
	//printf("%s", buffer);
	//printf("Parent: %d\n", getpid());
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
		//printf("That's a nice pipe you got there...\n");
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
	if(builtin !=0 && builtin != -1) {
		do_it();
		currcmd += 1;
		printf("HERE\n");
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
			/*
			int status;
			int pid = fork();
			if(pid < 0) {
				printf("Main fork Error\n");
			}
			else if(pid == 0) {
				*/
				//printPrompt();
				if(!pipeProcess) {
					printf("Proocess ID: %d\n", getpid());
					currcmd = 0;
					zeroStringArray('p');
					clearComTab();
					//printf("What the Fuck am I here for!?\n");
					cmd = getCommand();
					switch (cmd) {
						case BYE:		exit(0);
										break;
						case ERRORS:	exit(0);
										//recover_from_errors();
										break;
						case OK:		processCommand();
										break;
					}
				}
				//exit(1);//signal and then kill
			}

			//waitpid(pid, &status, NULL);//Parent wait and reloop to fork
		//}

	return 0;
}
