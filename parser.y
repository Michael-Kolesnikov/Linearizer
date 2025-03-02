%{
    #include <stdio.h>
	#include "ast.h"
	
    extern FILE *yyin;
    extern FILE *yyout;
    extern int yylex();
    int yyerror(const char *s);
    extern char *yytext;
    extern int lineno;
	#define YYDEBUG 1
	Node* root;
%}

%token	FUNC_NAME SIZEOF
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
%union{
	Node* node;
	char* str;
	int ival;
	double fval;
}
%token <str> IDENTIFIER 
%token <fval> F_CONST
%token <ival> I_CONST
%type <str> assignment_operator
%type <node> primary_expression direct_declarator declarator constant designator
%type <node> additive_expression multiplicative_expression cast_expression unary_expression postfix_expression shift_expression relational_expression equality_expression
%type <node> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <node> expression initializer 
%%
primary_expression
	: IDENTIFIER { $$ = create_identifier_node($1); }
	| constant
	| '(' expression ')' {$$ = $2;}
	;

constant
	: I_CONST { $$ = create_constant_int_node($1); }
	| F_CONST { $$ = create_constant_float_node($1); }
	;

postfix_expression
	: primary_expression { $$ = $1; }
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression POINTER_OP IDENTIFIER
	| postfix_expression INCR_OP
	| postfix_expression DECR_OP
	;

unary_expression
	: postfix_expression { $$ = $1; }
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
	: unary_expression { $$ = $1; }
	;

multiplicative_expression
	: cast_expression { $$ = $1; }
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = create_binary_operation_node('+',$1,$3); }
	| additive_expression '-' multiplicative_expression { $$ = create_binary_operation_node('-',$1,$3); }
	;

shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression { $$ = $1; }
	| unary_expression assignment_operator assignment_expression { root = create_assignment_node($1,$2,$3);}
	;

assignment_operator
	: '=' {$$ = "=";}
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
	: assignment_expression { $$ = $1; }
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;

declaration_specifiers
    : type_specifiers declaration_specifiers
    | type_specifiers
    ;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
    : declarator '=' initializer { create_declaration_node($1, $3); }
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
	| struct_or_union_specifier
	| TYPEDEF_NAME		
    ;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list ';'
	| specifier_qualifier_list struct_declarator_list ';'
	| static_assert_declaration
	;

specifier_qualifier_list
	: type_specifiers specifier_qualifier_list
	| type_specifiers
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator
	;

type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	| ATOMIC
	;

declarator
    : pointer direct_declarator
    | direct_declarator {$$ = $1;}
    ;

direct_declarator
    : IDENTIFIER {$$ = create_identifier_node($1); }
	| '(' declarator ')'
	| direct_declarator '[' ']'
    | direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')'
	 
    ;

pointer
    : '*'
    ;

parameter_type_list
	: parameter_list ',' ELLIPSIS
	| parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| direct_abstract_declarator '[' ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression {$$ = $1;}
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
	| '.' IDENTIFIER { $$ = create_identifier_node($2);}
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING ')' ';'
	;

statement
    : compound_statement
	| expression_statement
	| selection_statement
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

selection_statement
	: IF '(' expression ')' statement ELSE statement
	| IF '(' expression ')' statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	| FOR '(' declaration expression_statement ')' statement
	| FOR '(' declaration expression_statement expression ')' statement

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
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
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
    root->print(root);
	return 0;
}