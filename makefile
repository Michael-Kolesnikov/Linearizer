# Define the compiler and flags
CC = gcc
CFLAGS = -I. -Isrc -g
LDFLAGS = -lfl
TEST_CFLAGS = $(CFLAGS) -Iinclude -lcheck -lm -lsubunit

# Define source directories
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
TEST_DIR = tests

# Define the output executable name
TARGET = $(BIN_DIR)/parser

# Define source files
BISON_FILE = $(SRC_DIR)/parser.y
FLEX_FILE = $(SRC_DIR)/lexer.l
AST_FILE = $(SRC_DIR)/ast.c
CODEGEN_FILE = $(SRC_DIR)/codegeneration.c
SYMTAB_FILE = $(SRC_DIR)/symbolTable.c

# Generated files
BISON_C = $(BUILD_DIR)/parser.tab.c
BISON_H = $(BUILD_DIR)/parser.tab.h
FLEX_C = $(BUILD_DIR)/lex.yy.c
AST_O = $(BUILD_DIR)/ast.o
CODEGEN_O = $(BUILD_DIR)/codegeneration.o
SYMTAB_O = $(BUILD_DIR)/symbolTable.o
# Test source files
TEST_MAIN = $(TEST_DIR)/test_main.c


DIRS = $(BIN_DIR) $(BUILD_DIR)

$(DIRS):
	mkdir -p $@

# The default target
all: $(DIRS) $(TARGET)

# Rule to build the final executable
$(TARGET): $(BISON_C) $(FLEX_C) $(AST_O) $(CODEGEN_O) $(SYMTAB_O)
	$(CC) $(CFLAGS) -o $@ $(BISON_C) $(FLEX_C) $(AST_O) $(CODEGEN_O) $(SYMTAB_O) $(LDFLAGS)

# Rule to generate Bison files
$(BISON_C) $(BISON_H): $(BISON_FILE)
	bison -d $(BISON_FILE) --output-file=$(BISON_C) --defines=$(BISON_H)

# Rule to generate Flex files
$(FLEX_C): $(FLEX_FILE) $(BISON_H)
	flex --outfile=$(FLEX_C) $(FLEX_FILE)

# Rule to compile ast.c into ast.o
$(AST_O): $(AST_FILE)
	$(CC) $(CFLAGS) -c $(AST_FILE) -o $(AST_O)

# Rule to compile codegeneration.c into codegeneration.o
$(CODEGEN_O): $(CODEGEN_FILE)
	$(CC) $(CFLAGS) -c $(CODEGEN_FILE) -o $(CODEGEN_O)

$(SYMTAB_O): $(SYMTAB_FILE)
	$(CC) $(CFLAGS) -c $(SYMTAB_FILE) -o $(SYMTAB_O)

# Auto find all tests .c files
TEST_SRC := $(wildcard $(TEST_DIR)/*_test.c)
TEST_OBJ := $(patsubst %.c,%.o,$(TEST_SRC)) $(TEST_MAIN:.c=.o)


# Rule to build unit tests
unit_tests: $(TEST_OBJ) $(AST_O) $(CODEGEN_O) $(SYMTAB_O)
	$(CC) -o $(TEST_DIR)/unit_tests $(TEST_OBJ) $(AST_O) $(CODEGEN_O) $(SYMTAB_O) $(TEST_CFLAGS)
	./$(TEST_DIR)/unit_tests

# Rule to compile test source files into object files
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Rule to run all tests
test: unit_tests

# linting using clang-format
LINTER_FILES = $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(TEST_DIR)/*.c

lint:
	clang-format -i $(LINTER_FILES)
	
# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(BISON_C) $(BISON_H) $(FLEX_C) $(AST_O) $(CODEGEN_O) $(SYMTAB_O)\
	      $(TEST_DIR)/unit_tests $(TEST_DIR)/*.o

# Rule to run the program
run: $(TARGET)
	./$(TARGET) $(BUILD_DIR)/input.c $(BUILD_DIR)/output.c