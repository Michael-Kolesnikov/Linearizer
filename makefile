# Define the compiler and flags
CC = gcc
CFLAGS = -I. -g
LDFLAGS = -lfl
TEST_CFLAGS = $(CFLAGS) -Iinclude -lcheck -lm -lsubunit

# Define the output executable name
TARGET = parser

# Define source files
BISON_FILE = parser.y
FLEX_FILE = lexer.l
AST_FILE = ast.c
CODEGEN_FILE = codegeneration.c

# Generated files
BISON_C = parser.tab.c
BISON_H = parser.tab.h
FLEX_C = lex.yy.c
AST_O = ast.o
CODEGEN_O = codegeneration.o

# Test source files
TEST_DIR = tests
TEST_FUNC_DECL = $(TEST_DIR)/function_declaration_test.c
TEST_MAIN = $(TEST_DIR)/test_main.c
TEST_OBJ = $(TEST_FUNC_DECL:.c=.o) $(TEST_MAIN:.c=.o)

# The default target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(BISON_C) $(FLEX_C) $(AST_O) $(CODEGEN_O)
	$(CC) -o $@ $(BISON_C) $(FLEX_C) $(AST_O) $(CODEGEN_O) $(LDFLAGS)

# Rule to generate Bison files
$(BISON_C) $(BISON_H): $(BISON_FILE)
	bison -d $(BISON_FILE)

# Rule to generate Flex files
$(FLEX_C): $(FLEX_FILE) $(BISON_H)
	flex $(FLEX_FILE)

# Rule to compile ast.c into ast.o
$(AST_O): $(AST_FILE)
	$(CC) $(CFLAGS) -c $(AST_FILE) -o $(AST_O)

# Rule to compile codegeneration.c into codegeneration.o
$(CODEGEN_O): $(CODEGEN_FILE)
	$(CC) $(CFLAGS) -c $(CODEGEN_FILE) -o $(CODEGEN_O)

# Rule to build unit tests
unit_tests: $(TEST_OBJ) $(AST_O) $(CODEGEN_O)
	$(CC) -o $(TEST_DIR)/unit_tests $(TEST_OBJ) $(AST_O) $(CODEGEN_O) $(TEST_CFLAGS)
	./$(TEST_DIR)/unit_tests

# Rule to compile test source files into object files
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Rule to run all tests
test: unit_tests

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(BISON_C) $(BISON_H) $(FLEX_C) $(AST_O) $(CODEGEN_O) \
	      $(TEST_DIR)/unit_tests $(TEST_DIR)/*.o

# Rule to run the program
run: $(TARGET)
	./$(TARGET) input.c output.c