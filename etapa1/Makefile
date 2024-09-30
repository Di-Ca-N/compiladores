etapa1: main.c tokens.h lex.yy.c 
	$(CC) $^ -o $@ -lfl

lex.yy.c: scanner.l
	flex scanner.l

.PHONY: clean
clean:
	rm -f lex.yy.c etapa1
