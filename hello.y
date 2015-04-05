%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}
%}
%token CD STRING PWD
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

builtin:	CD STRING	{return 1;};

other:		PWD			{return 2;};

