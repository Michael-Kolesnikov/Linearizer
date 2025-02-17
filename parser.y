%{
    #include <stdio.h>
    extern FILE *yyin;
    extern FILE *yyout;
    extern int yylex();
    int yyerror(const char *s);
    extern char *yytext;
    extern int lineno;
	#define YYDEBUG 1
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
primary_expression
	: IDENTIFIER
	| constant
	| '(' expression ')'
	;

constant
	: I_CONST
	| F_CONST
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression POINTER_OP IDENTIFIER
	| postfix_expression INCR_OP
	| postfix_expression DECR_OP
	;

unary_expression
	: postfix_expression
	| INCR_OP unary_expression
	| DECR_OP unary_expression
	| unary_operator cast_expression
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

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
    : declarator '=' initializer	
	| declarator
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
	| '(' declarator ')'
    | direct_declarator '(' ')'
	
    ;

pointer
    : '*'
    ;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;
statement
    : compound_statement
	| expression_statement
	| iteration_statement
	| jump_statement
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

expression_statement
	: ';'
	| expression ';'
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
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
	yydebug = 1;
    yyparse();
    fclose(yyin);
    fclose(yyout);

    return 0;
}