TARGET = etapa2
SRC := src
OBJ := build

SOURCES := $(wildcard $(SRC)/*.c) $(SRC)/lex.yy.c $(SRC)/parser.tab.c 
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES)) $(OBJ)/lex.yy.o $(OBJ)/parser.tab.o  

$(TARGET): $(OBJECTS)
		$(CC) $^ -o $@ -lfl

$(SRC)/lex.yy.c: $(SRC)/scanner.l
	flex -o $@ $(SRC)/scanner.l

$(SRC)/parser.tab.c $(SRC)/parser.tab.h: $(SRC)/parser.y
	bison -d -b $(SRC)/parser $(SRC)/parser.y -Wall

$(OBJ)/main.o: $(SRC)/main.c $(SRC)/parser.tab.h

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) -I$(SRC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(SRC)/lex.yy.c $(TARGET) $(SRC)/parser.tab.c $(SRC)/parser.tab.h $(OBJ)/*.o $(OBJ)
