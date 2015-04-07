%{#include <stdio.h>
#include "y.tab.h"
#include "shell.h"
%}
%%
cd			return CD;
bye			return EXIT;
cd+[ ]+[a-zA-Z0-9/-_]+ 	{	
							strcpy(stringArray, yytext);
							return CDSTRING;
						}
[a-zA-Z0-9\_ \|\<\>\&\"\\]+	{
					/*printf("yytext: %s\n", yytext);*/
					strcpy(stringArray, yytext);
					//printf("stringArray: %s\n", yytext);
					return STRING;
				}
[ \t]+	/*ignore whitespace*/;

\n 			/* ignore end of line */
%%