%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}

void removeAllChars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

int isQuote(char c) {
	if(c == '\"') {
		return TRUE;
	}
	return FALSE;
}

int hasQuote(char c[]) {
	int length = strlen(c);
	int numQuotes = 0;
	int i;
	for(i = 0; i < length; i++) {
		if(isQuote(c[i])) {
			numQuotes++;
		}
	}
	return numQuotes;
}

int isMetaChar(char c[]) {
	if(	strcmp(c, "|") 		== 0
		|| strcmp(c, "|") 	== 0
		|| strcmp(c, "<") 	== 0
		|| strcmp(c, ">") 	== 0
		|| strcmp(c, "&") 	== 0
		|| strcmp(c, "\\") 	== 0
		|| strcmp(c, "<<") 	== 0
		|| strcmp(c, ">>") 	== 0)
	{
		// found meta char
		return TRUE;
	}
	return FALSE;
}

void parseCommand()	{
	char* token;
	token = strtok(stringArray, " ");
	
	int currCommandIndex = -1;
	int currArgsIndex = -1;
	
	int j = 0;
	int k = 0;
	
	while(token != NULL) {
		if(isAlias(token)) {
			// expand alias
		}
		if(!isMetaChar(token)) {
			if(j == 0) {
				currCommandIndex++;
				comtab[currCommandIndex].name = token;
				comtab[currCommandIndex].numArgs = 0;
				j++;
				currArgsIndex = -1;
			}
			else if(hasQuote(token) == 1) {
				char* space = " ";
				
				while(token != NULL) {
					if(k > 0) {
						strcat(quotedString, space);
					}
					strcat(quotedString, token);
					
					token = strtok(NULL, " ");
					if(token == NULL || hasQuote(token) == 1) {
						strcat(quotedString, space);
						strcat(quotedString, token);
						break;
					}
					k++;
				}
				currArgsIndex++;
				comtab[currCommandIndex].args[currArgsIndex] = quotedString;
				removeAllChars(comtab[currCommandIndex].args[currArgsIndex], '\"');
				comtab[currCommandIndex].numArgs = currArgsIndex + 1;
			}
			else {
				currArgsIndex++;
				comtab[currCommandIndex].args[currArgsIndex] = token;
				removeAllChars(comtab[currCommandIndex].args[currArgsIndex], '\"');
				comtab[currCommandIndex].numArgs = currArgsIndex + 1;
			}
		}
		else {
			if(strcmp(token, "|") == 0) {
				comtab[currCommandIndex].hasPipe = TRUE;
			}
			if(strcmp(token, "<") == 0) {
				comtab[currCommandIndex].hasIRed = TRUE;
			}
			if(strcmp(token, ">") == 0) {
				comtab[currCommandIndex].hasORed = TRUE;
			}
			if(strcmp(token, "&") == 0) {
				// do nothing
			}
			if(strcmp(token, "\\") == 0) {
				// do nothing
			}
			if(strcmp(token, "<<") == 0) {
				comtab[currCommandIndex].hasIRed = TWO;
			}
			if(strcmp(token, ">>") == 0) {
				comtab[currCommandIndex].hasORed = TWO;
			}
			j = 0;
		}
		if(token != NULL) {
			token = strtok (NULL, " ");
		}
	}
}

void printAliases() {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(aliastab[index].name[0] != '\0') {
			printf("%s\t", aliastab[index].name);
			printf("%s\n", aliastab[index].str);
		}
	}
}

void addAlias() {
	int index = findNextAliasIndex();
	
	char* token;
	token = strtok(stringArray, " "); // token is now "alias"
	token = strtok(NULL, " "); // token is now the alias name
	
	if(isAlias(token) == TRUE) {
		printf("Alias is already taken\n");
		return;
	}
	strcpy(aliastab[index].name, token);
	
	token = strtok(NULL, " "); // token is now the alias string
	
	int k = 0;
	char* space = " ";
	
	if(hasQuote(token) == 1) {
		while(token != NULL) {
			if(k > 0) {
				strcat(quotedString, space);
			}
			strcat(quotedString, token);
			
			token = strtok(NULL, " ");
			if(token == NULL || hasQuote(token) == 1) {
				strcat(quotedString, space);
				strcat(quotedString, token);
				break;
			}
			k++;
		}
		strcpy(aliastab[index].str, quotedString);
		removeAllChars(aliastab[index].str, '\"');
	}
	else {
		strcpy(aliastab[index].str, token);
		removeAllChars(aliastab[index].str, '\"');
		return;
	}
}

int isAlias(char* alias) {
	// check if alias already exists
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(strcmp(aliastab[index].name, alias) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

int findNextAliasIndex() {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(aliastab[index].name[0] == '\0') {
			return index;
		}
	}
	return -1;
}

char* findAliasString(char* name) {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(strcmp(aliastab[index].name, name) == 0) {
			return aliastab[index].str;
		}
	}
	return NULL;
}

void removeAlias() {
	char* token;
	token = strtok(stringArray, " "); // token is now "unalias"
	token = strtok(NULL, " "); // token is now the alias name
	
	if(isAlias(token) == FALSE) {
		printf("\"%s\" is not an alias\n", token);
		return;
	}
	
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(strcmp(aliastab[index].name, token) == 0) {
			aliastab[index].name[0] = '\0';
			aliastab[index].str[0] = '\0';
		}
	}
}

%}
%token CD STRING EXIT CDSTRING ALIAS ALIASSTRING ALIASCOMMAND UNALIAS SETENV PRINTENV UNSETENV
%start cmd
%%

cmd: 	builtin
	| 	other;

builtin:	CDSTRING		{return 5;}
		|	CD   			{return 6;}
		|	ALIASCOMMAND	{addAlias(); return 9;}
		|	ALIASSTRING		{addAlias(); return 7;}
		|	ALIAS			{printAliases(); return 8;}
		|	UNALIAS			{removeAlias(); return 10;}
		|	SETENV			{return 11;}
		|	PRINTENV		{return 12;}
		|	UNSETENV		{return 13;}
		|	EXIT			{return BYE;};

other:	STRING				{parseCommand(); return -1;};
