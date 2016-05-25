CFLAGS=-D DEBUG

pract05: parser.tab.o lex.yy.o
	g++ -o project parser.tab.o lex.yy.o -ll

.c.o: parser.tab.h
	g++ -c $< $(CFLAGS)

parser.tab.c: parser.y
	bison -d parser.y

parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexical.l
	flex lexical.l

clean:
	rm -f *.o *~ parser.tab.* lex.yy.c project
