#include <string.h>
#include <unistd.h>

#define TWO				2
#define TRUE			1
#define FALSE			0

#define MAXCMDS 		50
#define MAXARGS			300
#define MAXALIAS		100
#define MAXPATH			50

#define OK				0
#define SYSERR 			1
#define ERRORS			2
#define BYE				4

typedef struct com {
	char* name;
	int hasPipe;
	int hasIRed;
	int hasORed;
	int hasAmpersand;
	int infd;
	int outfd;
	int numArgs;
	char* args[MAXARGS];
} COMMAND;

/* alias structure */
typedef struct alias {
	int used;
	char name[500];
	char str[500];
} MYALIAS;

/* externals */
extern MYALIAS aliastab[];
extern COMMAND comtab[];
extern int currcmd;
extern int currarg;
extern int builtin;
extern void displacedStringArray(int j);
extern char **environ;

char quotedString[500];
char stringArray[MAXPATH];
char cwd[MAXPATH];
