%{#include <stdio.h>
#include "y.tab.h"
#include "shell.h"
%}
%%
ls			return LS;
pwd			return PWD;
cd			return CD;
ls+[ ]+[a-zA-Z0-9/-_]+	{
							strcpy(stringArray, yytext);
							return LSSTRING;
						}
echo+[ ]+[a-zA-Z0-9 ]+	{
							strcpy(stringArray, yytext);
							return ECHO;
						}
mkdir		return MKDIR;
bye			return EXIT;
cd+[ ]+[a-zA-Z0-9/-_]+ 	{	
							strcpy(stringArray, yytext);
							return CDSTRING;
						}
[a-zA-Z0-9/]+	{
					printf("yytext: %s\n", yytext);
					strcpy(stringArray, yytext);
					return STRING;
				}
[ \t]+	/*ignore whitespace*/;

\n 			/* ignore end of line */
%%