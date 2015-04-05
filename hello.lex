%{#include <stdio.h>
#include "y.tab.h"
#include "shell.h"
%}
%%
pwd			return PWD;
cd			return CD;
[a-zA-Z0-9/]+ 	{
					strcpy(stringArray, yytext);
					return STRING;
				}
[ \t]+	/*ignore whitespace*/;

\n 			/* ignore end of line */
%%
