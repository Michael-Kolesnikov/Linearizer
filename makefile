# Define the compiler and flags
CC = gcc
CFLAGS = -I. -g
LDFLAGS = -lfl

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

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(BISON_C) $(BISON_H) $(FLEX_C) $(AST_O) $(CODEGEN_O)

# Rule to run the program
run: $(TARGET)
	./$(TARGET) input.c output.c