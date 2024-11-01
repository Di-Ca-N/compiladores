TARGET = etapa3
SRC := src
INC := include
OBJ := build
CFLAGS := -Wall -Wextra

GENERATED := $(SRC)/lex.yy.c $(SRC)/parser.tab.c
SOURCES := $(wildcard $(SRC)/*.c) $(GENERATED)
REAL_SOURCES := $(filter-out $(GENERATED), $(SOURCES))
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
DEPS := $(patsubst $(SRC)/%.c, $(OBJ)/%.d, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ -lfl $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) -MMD -I$(INC) -c $(SRC)/$*.c -o $@ $(CFLAGS)

-include $(OBJ)/*.d

$(REAL_SOURCES): $(GENERATED) 

$(SRC)/lex.yy.c: $(SRC)/scanner.l
	flex -o $@ $^

$(SRC)/parser.tab.c $(INC)/parser.tab.h: $(SRC)/parser.y
	bison -d -b $(SRC)/parser $< -Wall
	mv $(SRC)/parser.tab.h $(INC)/parser.tab.h 

echo:
	@echo $(REAL_SOURCES)
	@echo $(GENERATED)
	@echo $(OBJECTS)
	@echo $(DEPS)

.PHONY: clean
clean:
	rm -rf $(SRC)/lex.yy.c $(TARGET) $(SRC)/parser.tab.c $(INC)/parser.tab.h $(OBJ)
