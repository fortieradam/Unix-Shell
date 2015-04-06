#include <string.h>


#define TRUE			1
#define FALSE			0

#define MAXCMDS 		50
#define MAXARGS			300
#define MAXALIAS		100
#define MAXCACHE		100
#define MAXPATH			50

#define OK				0
#define SYSERR 			1
#define ERRORS			2
#define OLD_ERR			3
#define SYSCALLERR		-1
#define BYE				4

#define BADFD			-2
#define EOFILE			2

#define THE_ONLY_ONE	1
#define FIRST			2
#define LAST			3

#define REGISTRY		"REGISTRY"

/* command line arguments structure */
typedef struct comargs {
	char *args[MAXARGS];
} ARGTAB;

/* command line structure */
/*typedef struct com {
	char *comname;
	int remote;
	int infd;
	int outfd;
	int nargs;
	ARGTAB *atptr;
} COMMAND;*/

typedef struct com {
	char* code;
	int hasPipe;
	int hasIORed;
	int infd;
	int outfd;
	int numArgs;
	ARGTAB* argPtr;
} COMMAND;

/* alias structure */
struct alias {
	int used;
	char *alname;
	char *alstr;
};

/* cache table structure */
typedef struct cache {
	char *cmd;
	char *hostport;
} CACHE;

/* externals */
extern struct alias aliastab[];
extern COMMAND comtab[];
extern CACHE cachetab[];
extern char *pathtab[];
extern int currcmd;
extern int currcache;
extern int lastcurrcmd;
extern int recursive;
extern int pathleng;
extern int currarg;
extern int login;
extern int builtin;
extern int bicmd;
extern int bioutf;
extern char *bistr;
extern char *bistr2;
extern int debug;
extern int IwasSet;
extern int err;
extern int error_somewhere;
extern int ap;
extern int cmd_has_tail;
extern int backgnd;
extern int concurrp;
extern int eventcount;
extern char home[];
extern char *shname;
extern char *prompt_string;
extern int user_defined_prompt;
extern char srcf[];
extern char distf[];
extern int append;
extern int loop;
extern void displacedStringArray(int j);
extern COMMAND* initializeCom(int code, int hasPipe, int hasIORed, int infd, int outfd, int numArgs, ARGTAB* argPtr);


#define NIL(x) 				(x *)0
#define ALL 				NIL(char)
#define copystring(a, b)	strcpy((a=(char *)malloc(strlen(b)+1)),b)
#define mkstr(a)			(char *)makestring(a)
#define Allocate(t)			(t *)malloc(sizeof(t))


char stringArray[MAXPATH];
char cwd[MAXPATH];
