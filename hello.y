%{#include <stdio.h>
#include "shell.h"
#include <assert.h>
#include <stdlib.h>

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
	char* pch;
	printf("Splitting string \"%s\" into tokens:\n", stringArray);
	pch = strtok(stringArray, " ");
	
	int j = 0;
	
	while(pch != NULL) {
		printf ("pch = %s\n", pch);
		
		if(!isMetaChar(pch)) {
			if(j == 0) {
				printf("\tfound command!\n");
				j++;
			}
			else {
				printf("\tfound arg!\n");
			}
		}
		else {
			if(strcmp(pch, "|") == 0) {
				printf("\tfound pipe!\n");
			}
			if(strcmp(pch, "<") == 0) {
				printf("\tfound less than!\n");
			}
			if(strcmp(pch, ">") == 0) {
				printf("\tfound greater than!\n");
			}
			if(strcmp(pch, "&") == 0) {
				printf("\tfound ampersand!\n");
			}
			if(strcmp(pch, "\"") == 0) {
				printf("\tfound quote!\n");
			}
			if(strcmp(pch, "\\") == 0) {
				printf("\tfound forward slash!\n");
			}
			j = 0;
		}
		
		pch = strtok (NULL, " ");
	}
}

%}
%token CD STRING PWD LS ECHO MKDIR EXIT CDSTRING LSSTRING
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
		|	LS			{return 10;}
		|	LSSTRING	{return 11;}
		|	EXIT		{return BYE;};


other:		PWD				{return 9;}
		|	ECHO			{return 7;}
		|	MKDIR STRING 	{return 8;}
		|	STRING			{parseCommand(); return 12;};

