%{#include <stdio.h>
#include "y.tab.h"
#include "shell.h"
%}
%%
alias				return ALIAS;
cd					return CD;
bye					return EXIT;
alias+[ ]+[a-zA-Z]++[ ]+[a-zA-Z]+	{
									strcpy(stringArray, yytext);
									return ALIASCOMMAND;
								}		
alias+[ ]+[a-zA-Z]++[ ]+[\"]+[a-zA-Z0-9/\.\-\_ \|\<\>\&\"\\]++[\"]	{
																		strcpy(stringArray, yytext);
																		return ALIASSTRING;
																	}
cd+[ ]+[a-zA-Z0-9/\-\_]+ 	{	
								strcpy(stringArray, yytext);
								return CDSTRING;
							}
[a-zA-Z0-9/\.\-\_ \|\<\>\&\"\\]+	{
										/*printf("yytext: %s\n", yytext);*/
										strcpy(stringArray, yytext);
										//printf("stringArray: %s\n", yytext);
										return STRING;
									}
[ \t]+	/*ignore whitespace*/;

\n 		/* ignore end of line */
%%