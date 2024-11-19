TARGET = etapa4
SRC := src
INC := include
BUILD := build
BUILD_CODE := $(BUILD)/code
BUILD_OBJ := $(BUILD)/obj
BUILD_DEPS := $(BUILD)/deps
CFLAGS := -Wall -Wextra

GENERATED := $(BUILD_CODE)/lex.yy.c $(BUILD_CODE)/parser.tab.c
SOURCES := $(wildcard $(SRC)/*.c)

SOURCE_OBJ := $(patsubst $(SRC)/%.c, $(BUILD_OBJ)/%.o, $(SOURCES))
GEN_OBJ := $(patsubst $(BUILD_CODE)/%.c, $(BUILD_OBJ)/%.o, $(GENERATED))
OBJECTS := $(SOURCE_OBJ) $(GEN_OBJ)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ -lfl $(CFLAGS)

$(BUILD_OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(BUILD_OBJ) $(BUILD_DEPS)
	$(CC) -MMD -MF $(BUILD_DEPS)/$*.d -I$(INC) -I$(BUILD_CODE) -c $< -o $@ $(CFLAGS)

$(BUILD_OBJ)/%.o: $(BUILD_CODE)/%.c
	@mkdir -p $(BUILD_OBJ) $(BUILD_DEPS)
	$(CC) -MMD -MF $(BUILD_DEPS)/$*.d -I$(INC) -I$(BUILD_CODE) -c $< -o $@ $(CFLAGS)

-include $(BUILD_DEPS)/*.d

$(SOURCES): $(GENERATED) 

$(BUILD_CODE)/lex.yy.c: $(SRC)/scanner.l $(BUILD_CODE)/parser.tab.c
	@mkdir -p $(BUILD_CODE)
	flex -o $@ $<

$(BUILD_CODE)/parser.tab.c: $(SRC)/parser.y
	@mkdir -p $(BUILD_CODE)
	bison -d -b $(BUILD_CODE)/parser $< -Wall

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BUILD)
