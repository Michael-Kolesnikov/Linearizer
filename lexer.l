%{
#include "parser.tab.h"
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
"if" {yylval.sval = strdup(yytext); printf("IF"); return IF; }
"else" {yylval.sval = strdup(yytext); printf("ELSE"); return ELSE; }
"int" {yylval.sval = strdup(yytext); printf("INT "); return INT; }
"short" {yylval.sval = strdup(yytext); printf("SHORT"); return SHORT; }
"char" {yylval.sval = strdup(yytext); printf("CHAR"); return CHAR; }
"long" {yylval.sval = strdup(yytext); printf("LONG"); return LONG; }
"float" {yylval.sval = strdup(yytext); printf("FLOAT"); return FLOAT; }
"double" {yylval.sval = strdup(yytext); printf("DOUBLE"); return DOUBLE; }
"signed" {yylval.sval = strdup(yytext); printf("SIGNED"); return SIGNED; }
"usigned" {yylval.sval = strdup(yytext); printf("UNSIGNED"); return UNSIGNED; }
"const" {yylval.sval = strdup(yytext); printf("CONST"); return CONST; }

[a-zA-Z_][a-zA-Z0-9_]*   {
    if (is_keyword(yytext)) {
        yylval.sval = strdup(yytext);
        printf("KEYWORD ");
        return KEYWORD;
    } else {
        yylval.sval = strdup(yytext);
        printf("IDENTIFIER ");
        return IDENTIFIER;
    }
}

"#"[a-zA-Z]+.*          {yylval.sval = strdup(yytext); printf("PREPROCESSOR "); return PREPROCESSOR; }
"//".*                  {yylval.sval = strdup(yytext); printf("COMMENT "); return COMMENT; }
"/\\*".*?"\\*/"         {yylval.sval = strdup(yytext); printf("COMMENT "); return COMMENT; }

\"(\\.|[^"])*\"          {yylval.sval = strdup(yytext); printf("STRING "); return STRING; }
\'(\\.|[^'])\'           {yylval.sval = strdup(yytext); printf("CHARACTER "); return CHARACTER; }

[0-9]+ {yylval.sval = strdup(yytext); printf("NUMBER "); return NUMBER; }
[0-9]+\.[0-9]+ {yylval.sval = strdup(yytext); printf("FLOAT "); return FLOAT; }


";"                     {   printf(";"); return(';');    }
("{"|"<%")              {   return('{');    }
("}"|"%>")              {   return('}');    }
","                     {   return(',');    }
":"                     {   return(':');    }
"="                     {   return('=');    }
"("                     {   return('(');    }
")"                     {   return(')');    }
("["|"<:")              {   return('[');    }
("]"|":>")              {   return(']');    }
"."                     {   return('.');    }
"&"                     {   return('&');    }
"!"                     {   return('!');    }
"~"                     {   return('~');    }
"-"                     {   return('-');    }
"+"                     {   return('+');    }
"*"                     {   return('*');    }
"/"                     {   return('/');    }
"%"                     {   return('%');    }
"<"                     {   return('<');    }
">"                     {   return('>');    }
"^"                     {   return('^');    }
"|"                     {   return('|');    }
"?"                     {   return('?');    }

"=="|"!="|"<="|">="     {yylval.sval = strdup(yytext);printf("OPERATOR "); return OPERATOR; }

[ \t\n\r]+ ;
. ;

%%

