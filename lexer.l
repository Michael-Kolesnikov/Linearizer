%{
// #include "parser.tab.h"
#include <stdio.h>
#include <string.h>

#define MAX_KEYWORDS 32

// Keywords in C
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary"
};

// Check if a word is a keyword
int is_keyword(const char* word) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

%}

%option noyywrap

%%

[a-zA-Z_][a-zA-Z0-9_]*   {
    if (is_keyword(yytext)) {
        printf("KEYWORD: %s\n", yytext);
    } else {
        printf("IDENTIFIER: %s\n", yytext);
    }
}

"#"[a-zA-Z]+.*          { printf("PREPROCESSOR: %s\n", yytext); }
"//".*                  { printf("COMMENT: %s\n", yytext); }
"/\\*".*?"\\*/"         { printf("COMMENT: %s\n", yytext); }

\"(\\.|[^"])*\"          { printf("STRING: %s\n", yytext); }
\'(\\.|[^'])\'           { printf("CHARACTER: %s\n", yytext); }

[0-9]+ { printf("NUMBER: %s\n", yytext); }
[0-9]+\.[0-9]+ { printf("FLOAT: %s\n", yytext); }

"=="|"!="|"<="|">="     { printf("OPERATOR: %s\n", yytext); }
[+\-*/%<>=&|!~^]+       { printf("OPERATOR: %s\n", yytext); }
[{}();,.\[\]]           { printf("PUNCTUATION: %s\n", yytext); }

[ \t\n\r]+ ;

. { printf("UNKNOWN: %s\n", yytext); }

%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            return 1;
        }
        // Set the input stream for Flex
        yyin = file;
    }
    yylex();
    return 0;
}