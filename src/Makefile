all:
	bison -d parser.y
	flex scanner.l
	gcc -o parser.o parser.tab.c lex.yy.c -lfl
	rm -f parser.tab.c parser.tab.h lex.yy.c