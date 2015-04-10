%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}

int isMetaChar(char c[]) {
	if(	strcmp(c, "|") 		== 0
		|| strcmp(c, "|") 	== 0
		|| strcmp(c, "<") 	== 0
		|| strcmp(c, ">") 	== 0
		|| strcmp(c, "&") 	== 0
		|| strcmp(c, "\"") 	== 0
		|| strcmp(c, "\\") 	== 0
		|| strcmp(c, "<<") 	== 0
		|| strcmp(c, ">>") 	== 0)
	{
		//printf("found meta char\n");
		return TRUE;
	}
	return FALSE;
}

void parseCommand()	{
	char* token;
	//printf("Splitting string \"%s\" into tokens:\n", stringArray);
	token = strtok(stringArray, " ");
	
	int currCommandIndex = -1;
	int currArgsIndex = -1;
	
	int j = 0;
	
	while(token != NULL) {
		//printf ("token = %s\n", token);
		
		if(!isMetaChar(token)) {
			if(j == 0) {
				currCommandIndex++;
				comtab[currCommandIndex].name = token;
				//printf("\tcomtab[%d].name = %s\n", currCommandIndex,comtab[currCommandIndex].name);
				comtab[currCommandIndex].numArgs = 0;
				j++;
			}
			else {
				currArgsIndex++;
				comtab[currCommandIndex].args[currArgsIndex] = token;
				comtab[currCommandIndex].numArgs = currArgsIndex + 1;
				//printf("\tcomtab[%d].args[%d] = %s\n", currCommandIndex, currArgsIndex, comtab[currCommandIndex].args[currArgsIndex]);
			}
		}
		else {
			if(strcmp(token, "|") == 0) {
				comtab[currCommandIndex].hasPipe = TRUE;
				//printf("\tcomtab[%d].hasPipe = %d\n", currCommandIndex, comtab[currCommandIndex].hasPipe);
			}
			if(strcmp(token, "<") == 0) {
				comtab[currCommandIndex].hasIRed = TRUE;
				//printf("\tcomtab[%d].hasIRed = %d\n", currCommandIndex, comtab[currCommandIndex].hasIRed);
			}
			if(strcmp(token, ">") == 0) {
				comtab[currCommandIndex].hasORed = TRUE;
				//printf("\tcomtab[%d].hasORed = %d\n", currCommandIndex, comtab[currCommandIndex].hasORed);
			}
			if(strcmp(token, "&") == 0) {
				//printf("\tfound ampersand!\n");
				//printf("\tcomtab[%d].hasAmpersand = %d\n", currCommandIndex, comtab[currCommandIndex].hasAmpersand);
			}
			if(strcmp(token, "\"") == 0) {
				//printf("\tfound quote!\n");
			}
			if(strcmp(token, "\\") == 0) {
				//printf("\tfound forward slash!\n");
			}
			if(strcmp(token, "<<") == 0) {
				comtab[currCommandIndex].hasIRed = TWO;
				//printf("\tcomtab[%d].hasIRed = %d\n", currCommandIndex, comtab[currCommandIndex].hasIRed);
			}
			if(strcmp(token, ">>") == 0) {
				comtab[currCommandIndex].hasORed = TWO;
				//printf("\tcomtab[%d].hasORed = %d\n", currCommandIndex, comtab[currCommandIndex].hasORed);
			}
			j = 0;
		}
		
		token = strtok (NULL, " ");
	}
	
	currCommandIndex = 0;
	currArgsIndex = 0;
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

