# Nome do executável final exigido pelo professor
TARGET = g-v1

all: $(TARGET)

$(TARGET): lex.yy.c g-v1.tab.c
	gcc -o $(TARGET) g-v1.tab.c lex.yy.c

lex.yy.c: g-v1.l
	flex g-v1.l

g-v1.tab.c: g-v1.y
	bison -d -v g-v1.y

clean:
	rm -f $(TARGET) lex.yy.c g-v1.tab.c g-v1.tab.h