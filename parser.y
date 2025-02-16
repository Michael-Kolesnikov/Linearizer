%{
    #include <stdio.h>
    extern FILE *yyin;
    extern FILE *yyout;
    extern int yylex();
    int yyerror(const char *s);
    extern char *yytext;
    extern int lineno;

%}

%token	IDENTIFIER I_CONST F_CONST FUNC_NAME SIZEOF
%token	POINTER_OP INCR_OP DECR_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL
%token COMMENT STRING CHARACTER PREPROCESSOR
%start program
%%

declaration
    : decloration_specifiers ';'
    | decloration_specifiers init_declarator_list ';'
    ;

decloration_specifiers
    : type_specifiers decloration_specifiers
    | type_specifiers
    ;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
    : declarator
    | direct_declarator '(' ')'
    ;

type_specifiers
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| BOOL
	| COMPLEX
	| IMAGINARY
	| TYPEDEF_NAME		
    ;

declarator
    : pointer direct_declarator
    | direct_declarator
    ;

direct_declarator
    : IDENTIFIER
    | direct_declarator '(' ')'
    ;

pointer
    : '*'
    ;

statement
    : compound_statement
    ;

compound_statement
	: '{' '}'
	| '{'  block_item_list '}'
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

program
	: external_declaration
	| program external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
    : decloration_specifiers declarator declaration_list compound_statement
    | decloration_specifiers declarator compound_statement
    ;

declaration_list
	: declaration
	| declaration_list declaration
	;
%%


int yyerror(const char *s) {
    fprintf(stderr, "Error at line: %d %s\n",lineno,s);
    fprintf(stderr, "Near token: '%s'\n", yytext);
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