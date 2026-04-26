# Nome do executável final
TARGET = g-v1

all: $(TARGET)

$(TARGET): lex.yy.c g-v1.tab.c ast.c symtab.c semantico.c codigo.c
	gcc -o $(TARGET) g-v1.tab.c lex.yy.c ast.c symtab.c semantico.c codigo.c

lex.yy.c: g-v1.l g-v1.tab.h
	flex g-v1.l

g-v1.tab.c g-v1.tab.h: g-v1.y
	bison -d -v g-v1.y

clean:
	rm -f $(TARGET) lex.yy.c g-v1.tab.c g-v1.tab.h g-v1.output