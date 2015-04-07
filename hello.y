%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}

int isMetaChar(char* c) {
	if(strcmp(c, "|") == 0
		|| strcmp(c, "|") == 0
		|| strcmp(c, "<") == 0
		|| strcmp(c, ">") == 0
		|| strcmp(c, "&") == 0
		|| strcmp(c, "\"") == 0
		|| strcmp(c, "\\") == 0)
	{
		return TRUE;
	}
	return FALSE;
}

void parseCommand()	{
	char* token;
	//printf("Splitting string \"%s\" into tokens:\n", stringArray);
	token = strtok(stringArray, " ");
	
	int numCommands = -1;
	int numArgs = -1;
	
	int j = 0;
	
	while(token != NULL) {
		//printf ("token = %s\n", token);
		
		if(!isMetaChar(token)) {
			if(j == 0) {
				numCommands++;
				comtab[numCommands].name = token;
				//printf("\tcomtab[%d].name = %s\n", numCommands,comtab[numCommands].name);
				j++;
			}
			else {
				numArgs++;
				comtab[numCommands].args[numArgs] = token;
				//printf("\tcomtab[%d].args[%d] = %s\n", numCommands, numArgs, comtab[numCommands].args[numArgs]);
			}
		}
		else {
			if(strcmp(token, "|") == 0) {
				comtab[numCommands].hasPipe = TRUE;
				//printf("\tcomtab[%d].hasPipe = %d\n", numCommands, comtab[numCommands].hasPipe);
			}
			if(strcmp(token, "<") == 0) {
				comtab[numCommands].hasIRed = TRUE;
				//printf("\tcomtab[%d].hasIRed = %d\n", numCommands, comtab[numCommands].hasIRed);
			}
			if(strcmp(token, ">") == 0) {
				comtab[numCommands].hasORed = TRUE;
				//printf("\tcomtab[%d].hasORed = %d\n", numCommands, comtab[numCommands].hasORed);
			}
			if(strcmp(token, "&") == 0) {
				//printf("\tfound ampersand!\n");
				//printf("\tcomtab[%d].hasAmpersand = %d\n", numCommands, comtab[numCommands].hasAmpersand);
			}
			if(strcmp(token, "\"") == 0) {
				//printf("\tfound quote!\n");
			}
			if(strcmp(token, "\\") == 0) {
				//printf("\tfound forward slash!\n");
			}
			j = 0;
		}
		
		token = strtok (NULL, " ");
	}
	
	numCommands = 0;
	numArgs = 0;
}

%}
%token CD STRING EXIT CDSTRING
%union {
	char* stringVal;
}
%start cmd
%%

commands: /* empty */
		| commands command;

command:
		cmd|builtin|other;

cmd: 	builtin
	| 	other;

builtin:	CDSTRING	{return 5;}
		|	CD   		{return 6;}
		|	EXIT		{return BYE;};


other:	STRING			{parseCommand(); return -1;};

