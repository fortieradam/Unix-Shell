%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}
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
		|	MKDIR STRING 	{return 8;};

