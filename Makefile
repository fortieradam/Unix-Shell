CC = gcc
CFLAGS = -g -Wall

default: shell

shell:  lex yacc lex.yy.o y.tab.o main.o
	$(CC) -o shell lex.yy.o y.tab.o main.o

lex:
	flex hello.lex
	
yacc:
	bison -dy hello.y

lex.yy.o:  lex.yy.c y.tab.h shell.h
	$(CC) -c lex.yy.c

y.tab.o:  y.tab.c shell.h
	$(CC) -c y.tab.c

main.o:  main.c shell.h
	$(CC) -c main.c

run:  shell
	./shell

clean:
	$(RM) shell *.o *~

zip:
	zip -r shell.zip main.c hello.lex hello.y shell.h Makefile README.txt