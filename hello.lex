%{#include <stdio.h>
#include "y.tab.h"
#include "shell.h"
%}
%%
ls			return LS;
pwd			return PWD;
cd			return CD;
[a-zA-Z0-9/]+ 	{
					strcpy(stringArray, yytext);
					return STRING;
				}
[ \t]+	/*ignore whitespace*/;

\n 			/* ignore end of line */
%%
