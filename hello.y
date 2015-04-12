%{#include <stdio.h>
#include "shell.h"

void yyerror(const char *str){fprintf(stderr,"error: %s\n",str);}
int yywrap(){return 1;}

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
		//|| strcmp(c, "\"") 	== 0
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
	int k = 0;
	
	while(token != NULL) {
		//printf ("token = %s\n", token);
		
		if(!isMetaChar(token)) {
			if(j == 0) {
				currCommandIndex++;
				comtab[currCommandIndex].name = token;
				printf("\tcomtab[%d].name = %s\n", currCommandIndex, comtab[currCommandIndex].name);
				comtab[currCommandIndex].numArgs = 0;
				j++;
			}
			else if(hasQuote(token) == 1) {
				char quotedString[500];
				char* space = " ";
				
				while(token != NULL) {
					if(k > 0) {
						strcat(quotedString, space);
						//printf("hit\n");
					}
					strcat(quotedString, token);
					
					printf("\t\ttoken = %s\n", token);
					printf("\t\tquotedString = %s\n", quotedString);
					
					token = strtok(NULL, " ");
					if(token == NULL || hasQuote(token) == 1) {
						strcat(quotedString, space);
						strcat(quotedString, token);
						
						printf("\t\ttoken = %s\n", token);
						printf("\t\tquotedString = %s\n", quotedString);
						
						break;
					}
					k++;
				}
				
				currArgsIndex++;
				comtab[currCommandIndex].args[currArgsIndex] = quotedString;
				comtab[currCommandIndex].numArgs = currArgsIndex + 1;
				printf("\tcomtab[%d].args[%d] = %s\n", currCommandIndex, currArgsIndex, comtab[currCommandIndex].args[currArgsIndex]);
			}
			else {
				currArgsIndex++;
				comtab[currCommandIndex].args[currArgsIndex] = token;
				comtab[currCommandIndex].numArgs = currArgsIndex + 1;
				printf("\tcomtab[%d].args[%d] = %s\n", currCommandIndex, currArgsIndex, comtab[currCommandIndex].args[currArgsIndex]);
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
		if(token != NULL) {
			token = strtok (NULL, " ");
		}
	}
}

void printAliases() {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(aliastab[index].name != NULL) {
			printf("%s\t\t", aliastab[index].name);
			printf("%s\n", aliastab[index].str);
		}
	}
}

void addAlias() {
	int index = findNextAliasIndex();
	
	char* token;
	//printf("Splitting string \"%s\" into tokens:\n", stringArray);
	
	token = strtok(stringArray, " "); // token is now "alias"
	token = strtok(NULL, " ");
	
	//printf("token = %s\n", token);
	
	if(isAlias(token)) {
		printf("Alias is already taken");
		return;
	}
	aliastab[index].name = token;
	
	printf("aliastab[%d].name = %s\n", index, aliastab[index].name);
	
	token = strtok(NULL, " "); // token is now the alias string
	
	int k = 0;
	char quotedString[500];
	char* space = " ";
	
	if(hasQuote(token) == 1) {
		while(token != NULL) {
			if(k > 0) {
				strcat(quotedString, space);
				//printf("hit\n");
			}
			strcat(quotedString, token);
			
			//printf("\t\ttoken = %s\n", token);
			//printf("\t\tquotedString = %s\n", quotedString);
			
			token = strtok(NULL, " ");
			if(token == NULL || hasQuote(token) == 1) {
				strcat(quotedString, space);
				strcat(quotedString, token);
				
				//printf("\t\ttoken = %s\n", token);
				//printf("\t\tquotedString = %s\n", quotedString);
				
				break;
			}
			k++;
		}
		aliastab[index].str = quotedString;
		printf("aliastab[%d].str = %s\n", index, aliastab[index].str);
	}
	else {
		aliastab[index].str = token;
		printf("aliastab[%d].str = %s\n", index, aliastab[index].str);
		return;
	}
}

int isAlias(char* alias) {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(aliastab[index].name == NULL) {
			//do nothing
		}
		else if(strcmp(aliastab[index].name, alias)) {
			return TRUE;
		}
	}
	return FALSE;
}

int findNextAliasIndex() {
	int index;
	for(index = 0; index < MAXALIAS; index++) {
		if(aliastab[index].name == NULL) {
			return index;
		}
	}
	return -1;
}

%}
%token CD STRING EXIT CDSTRING ALIAS ALIASSTRING ALIASCOMMAND
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

builtin:	CDSTRING		{return 5;}
		|	CD   			{return 6;}
		|	ALIASCOMMAND	{addAlias(); return 9;}
		|	ALIASSTRING		{addAlias(); return 7;}
		|	ALIAS			{printAliases(); return 8;}
		|	EXIT			{return BYE;};

other:	STRING				{parseCommand(); return -1;};
