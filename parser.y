%{
    #include <stdio.h>
	#include <stdlib.h>
	#include "ast.h"
	

    extern FILE *yyin;
    extern FILE *yyout;
	void generate_code_from_ast(Node* node, FILE* output);
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
%token	ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL
%token COMMENT CHARACTER PREPROCESSOR
%start program
%union{
	Node* node;
	char* str;
	int ival;
	double fval;
}
%token <str> IDENTIFIER INT VOID CHAR SHORT COMPLEX IMAGINARY BOOL LONG SIGNED UNSIGNED FLOAT DOUBLE TYPEDEF_NAME STRING
%token <fval> F_CONST
%token <ival> I_CONST
%type <str> assignment_operator declaration_specifiers type_specifiers
%type <node> primary_expression direct_declarator declarator constant designator
%type <node> additive_expression multiplicative_expression cast_expression unary_expression postfix_expression shift_expression relational_expression equality_expression
%type <node> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <node> expression initializer selection_statement expression_statement statement compound_statement block_item_list block_item
%type <node> init_declarator init_declarator_list declaration
%type <node> string iteration_statement labeled_statement constant_expression jump_statement
%%
primary_expression
	: IDENTIFIER { $$ = create_identifier_node($1); }
	| constant { $$ = $1; }
	| string { $$ = $1; }
	| '(' expression ')' {$$ = $2;}
	;

constant
	: I_CONST { $$ = create_constant_int_node($1); }
	| F_CONST { $$ = create_constant_float_node($1); }
	;

enumeration_constant
	: IDENTIFIER
	;

string
	: STRING { $$ = create_string_literal_node($1); }
	| FUNC_NAME
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
	| multiplicative_expression '*' cast_expression { $$ = create_binary_operation_node("*",$1,$3); }
	| multiplicative_expression '/' cast_expression { $$ = create_binary_operation_node("/",$1,$3); }
	| multiplicative_expression '%' cast_expression { $$ = create_binary_operation_node("%",$1,$3); }
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = create_binary_operation_node("+",$1,$3); }
	| additive_expression '-' multiplicative_expression { $$ = create_binary_operation_node("-",$1,$3); }
	;

shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression { $$ = create_binary_operation_node("<<",$1,$3); }
	| shift_expression RIGHT_OP additive_expression { $$ = create_binary_operation_node(">>",$1,$3); }
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression { $$ = create_logical_operation_node($1,"<", $3); }
	| relational_expression '>' shift_expression { $$ = create_logical_operation_node($1,">", $3); }
	| relational_expression LE_OP shift_expression { $$ = create_logical_operation_node($1,"<=", $3); }
	| relational_expression GE_OP shift_expression { $$ = create_logical_operation_node($1,">=", $3); }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQ_OP relational_expression { $$ = create_logical_operation_node($1,"==", $3); }
	| equality_expression NE_OP relational_expression { $$ = create_logical_operation_node($1,"!=", $3); }
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression { $$ = create_binary_operation_node("&",$1,$3); }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression { $$ = create_binary_operation_node("^",$1,$3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression { $$ = create_binary_operation_node("|",$1,$3); }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression { $$ = create_logical_operation_node($1,"&&", $3); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression { $$ = create_logical_operation_node($1,"||", $3); }
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression { $$ = create_ternary_operator_node($1, $3, $5); }
	;

assignment_expression
	: conditional_expression { $$ = $1; }
	| unary_expression assignment_operator assignment_expression { $$ = create_assignment_node($1,$2,$3);}
	;

assignment_operator
	: '=' {$$ = "=";}
	| MUL_ASSIGN { $$ = "*="; }
	| DIV_ASSIGN { $$ = "/="; }
	| MOD_ASSIGN { $$ = "%="; }
	| ADD_ASSIGN { $$ = "+="; }
	| SUB_ASSIGN { $$ = "-="; }
	| LEFT_ASSIGN { $$ = "<<="; }
	| RIGHT_ASSIGN { $$ = ">>="; }
	| AND_ASSIGN { $$ = "&="; }
	| XOR_ASSIGN { $$ = "^="; }
	| OR_ASSIGN { $$ = "|="; }
	;

expression
	: assignment_expression { $$ = $1; }
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression {$$ = $1; }
	;

declaration
    : declaration_specifiers ';' { $$ = $1;  }
    | declaration_specifiers init_declarator_list ';' {
		DeclarationNode* decl_node = (DeclarationNode*)$2;
		decl_node->type_specifier = strdup($1);
		$$ = $2;
		}
    ;

declaration_specifiers
    : type_specifiers declaration_specifiers
    | type_specifiers {$$ = $1;}
    ;

init_declarator_list
	: init_declarator { $$ = $1; }
	| init_declarator_list ',' init_declarator
	;

init_declarator
    : declarator '=' initializer { $$ = create_declaration_node($1, $3);}
	| declarator  { $$ = create_declaration_node($1, NULL);}
    ;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| THREAD_LOCAL
	| AUTO
	| REGISTER
	;

type_specifiers
	: VOID { $$ = $1; }
	| CHAR { $$ = $1; }
	| SHORT { $$ = $1; }
	| INT { $$ = $1; }
	| LONG { $$ = $1; }
	| FLOAT { $$ = $1; }
	| DOUBLE { $$ = $1; }
	| SIGNED { $$ = $1; }
	| UNSIGNED { $$ = $1; }
	| BOOL { $$ = $1; }
	| COMPLEX { $$ = $1; }
	| IMAGINARY { $$ = $1;}
	| struct_or_union_specifier
	| TYPEDEF_NAME { $$ = $1;}	
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

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: enumeration_constant '=' constant_expression
	| enumeration_constant
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
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'
    | direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')' { $$ = $1;}
    | direct_declarator '(' identifier_list ')'
	;
pointer
    : '*'
    ;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
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

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	| '[' STATIC assignment_expression ']'
	| '[' type_qualifier_list STATIC assignment_expression ']'
	| '[' type_qualifier_list assignment_expression ']'
	| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
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
    : labeled_statement { $$ = $1; }
	| compound_statement { $$ = $1; }
	| expression_statement { $$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1; }
    ;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement { $$ = create_case_node($2, $4); }
	| DEFAULT ':' statement { $$ = create_default_node($3); }
	;

compound_statement
	: '{' '}' { $$ = create_empty_statement_node(); }
	| '{'  block_item_list '}'{ $$ = $2;}
	;

block_item_list
	: block_item {
			Node** statements = (Node**)malloc(1 * sizeof(Node*));
			statements[0] = $1;
			$$ = create_compound_statement_node(statements,1);
		}
	| block_item_list block_item {
		CompoundStatementNode* prev_node = (CompoundStatementNode*)$1;
		Node** new_statements = (Node**)realloc(prev_node->statements, (prev_node->count + 1) * sizeof(Node*));
		new_statements[prev_node->count] = $2;
        
        prev_node->statements = new_statements;
        prev_node->count++;
        
        $$ = $1;
      }
	;

block_item
	: declaration { $$ = $1;}
	| statement { $$ = $1; }
	;

expression_statement
	: ';' { $$ = create_expression_statement_node(create_empty_statement_node());}
	| expression ';' { $$ = create_expression_statement_node($1); }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { $$ = create_if_node($3,$5, $7);}
	| IF '(' expression ')' statement  { $$ = create_if_node($3,$5,NULL);}
	| SWITCH '(' expression ')' statement { $$ = create_switch_node($3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = create_while_node($3,$5);}
	| DO statement WHILE '(' expression ')' ';' { $$ = create_do_while_node($2,$5); }
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	| FOR '(' declaration expression_statement ')' statement
	| FOR '(' declaration expression_statement expression ')' statement

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';' { $$ = create_break_node(); }
	| RETURN ';' { $$ = create_return_node(NULL); }
	| RETURN expression ';' { $$ = create_return_node($2); }
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
    | declaration_specifiers declarator compound_statement { root = create_function_declaration_node($1,$2,$3); }
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
	yydebug = 0;
    yyparse();

	if(root == NULL){
		printf("ROOT IS NULL\n");
	}else{
    	root->print(root);
		/* generate_code_from_ast(root,yyout); */
	}
	fclose(yyin);
    fclose(yyout);
	return 0;
}