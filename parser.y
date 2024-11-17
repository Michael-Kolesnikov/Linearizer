%{
#include <stdio.h>

int yylex();
int yyerror(const char *s);
int result;
%}


%token NUMBER

%%
term: NUMBER {result = NUMBER;}
;
%%


int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

