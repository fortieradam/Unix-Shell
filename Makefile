default: shell

shell: lex yacc main

lex:
	flex hello.lex
	
yacc:
	bison -dy hello.y

main:
	gcc lex.yy.c y.tab.c main.c -o shell

run: shell
	./shell

clean:
	rm shell lex.yy.c y.tab.c y.tab.h

zip:
	zip -r shell.zip main.c hello.lex hello.y shell.h Makefile README.txt