%{
#include <stdio.h>

extern FILE *yyin;
extern FILE *yyout;
int yylex();
int yyerror(const char *s);
extern char *yytext;
%}

%union {
    char *sval;
}

%token <sval> KEYWORD IDENTIFIER PREPROCESSOR COMMENT STRING CHARACTER NUMBER FLOAT OPERATOR PUNCTUATION

%type <sval> expression term
%%
program: /* Allow empty input */
       | statement_list;

statement_list: statement
              | statement_list statement;

statement: KEYWORD IDENTIFIER OPERATOR NUMBER PUNCTUATION {
              fprintf(yyout, "%s %s %s %s%s\n", $1, $2, $3, $4, $5);
              free($1);
              free($2);
              free($3);
              free($4);
              free($5);
          }
        | COMMENT {
              fprintf(yyout, "%s\n", $1);
              free($1);
          };
        | expression_statement


expression_statement: IDENTIFIER OPERATOR expression PUNCTUATION { asprintf(&$<sval>$, "%s %s %s;", $1, $2, $3);
    fprintf(yyout, "%s\n", $<sval>$);
    free($1);
    free($2);
    free($3);
    free($<sval>$);
}

expression: expression OPERATOR term {
    // Combine left expression, operator, and right term
    asprintf(&$<sval>$, "%s %s %s", $1, $2, $3);
    free($1);
    free($2);
    free($3);
}
| term {
    $$ = $1; // Pass through term (it's already a string)
};

term: IDENTIFIER {
    $$ = strdup($1);
}
| NUMBER {
    $$ = strdup($1);
}
| '(' expression ')' {
    asprintf(&$<sval>$, "(%s)", $2);  // Add parentheses around the expression
    free($2);
};
%%


int yyerror(const char *s) {
    fprintf(stderr, "Error: \n");
    return 0;
}

int main(int argc, char *argv[]){
    if(argc != 3) {
        printf("Error: Invalid argument. Usage: <source_file.c> <output_file.c>\n");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if(!yyin){
        perror("Error opening file");
        return 1;
    }
    yyout = fopen(argv[2],"w");
    if(!yyout){
        perror("Error opening ouput file");
        fclose(yyin);
        return 1;
    }

    yyparse();
    fclose(yyin);
    fclose(yyout);

    return 0;
}