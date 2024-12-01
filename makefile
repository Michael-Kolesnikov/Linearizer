# Define the compiler and flags
CC = gcc
CFLAGS = -lfl

# Define the output executable name
TARGET = parser

# Define source files
BISON_FILE = parser.y
FLEX_FILE = lexer.l

# Generated files
BISON_C = parser.tab.c
BISON_H = parser.tab.h
FLEX_C = lex.yy.c

# The default target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(BISON_C) $(FLEX_C)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to generate Bison files
$(BISON_C) $(BISON_H): $(BISON_FILE)
	bison -d $(BISON_FILE)

# Rule to generate Flex files
$(FLEX_C): $(FLEX_FILE)
	flex $(FLEX_FILE)

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(BISON_C) $(BISON_H) $(FLEX_C)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)
