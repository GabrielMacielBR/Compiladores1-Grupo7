CC = gcc
LEX = flex
YACC = bison -d

TARGET = parser

all: $(TARGET)

$(TARGET): parser.o lex.yy.o ast.o table.o
	$(CC) -o $(TARGET) parser.o lex.yy.o ast.o table.o -ll

parser.o: parser.tab.c ast.h table.h types.h
	$(CC) -c parser.tab.c -o parser.o

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c -o lex.yy.o

parser.tab.c parser.tab.h: parser.y
	$(YACC) parser.y

ast.o: ast.c ast.h types.h
	$(CC) -c ast.c -o ast.o

table.o: table.c table.h
	$(CC) -c table.c -o table.o

lex.yy.c: scanner.l
	$(LEX) scanner.l

clean:
	rm -f $(TARGET) *.o lex.yy.c parser.tab.c parser.tab.h