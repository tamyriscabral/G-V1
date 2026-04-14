# Nome do executável final exigido pelo professor
TARGET = g-v1

all: $(TARGET)

$(TARGET): lex.yy.c g-v1.tab.c ast.c ast.h symtab.c symtab.h
	gcc -o $(TARGET) g-v1.tab.c lex.yy.c ast.c symtab.c

lex.yy.c: g-v1.l g-v1.tab.h
	flex g-v1.l

g-v1.tab.c g-v1.tab.h: g-v1.y
	bison -d -v g-v1.y

clean:
	rm -f $(TARGET) lex.yy.c g-v1.tab.c g-v1.tab.h g-v1.output