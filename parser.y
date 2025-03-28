%{
    #include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
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

%token	INLINE
%token	ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF GENERIC NORETURN STATIC_ASSERT
%token COMMENT CHARACTER PREPROCESSOR
%start program
%union{
	Node* node;
	char* str;
	int ival;
	double fval;
	struct {
        struct ASTNode *node1;
        struct ASTNode *node2;
    } pair_node;
}
%token <str> IDENTIFIER INT VOID CHAR SHORT COMPLEX IMAGINARY BOOL LONG SIGNED UNSIGNED FLOAT DOUBLE TYPEDEF_NAME STRING STRUCT UNION
%token <str> TYPEDEF EXTERN STATIC THREAD_LOCAL AUTO REGISTER CONST RESTRICT ATOMIC VOLATILE
%token <fval> F_CONST
%token <ival> I_CONST
%type <str> assignment_operator declaration_specifiers unary_operator
%type <node> primary_expression direct_declarator declarator constant designator
%type <node> additive_expression multiplicative_expression cast_expression unary_expression postfix_expression shift_expression relational_expression equality_expression
%type <node> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <node> expression initializer selection_statement expression_statement statement compound_statement block_item_list block_item
%type <node> init_declarator init_declarator_list declaration
%type <node> string iteration_statement labeled_statement constant_expression jump_statement external_declaration function_definition abstract_declarator
%type <node> parameter_list parameter_declaration parameter_type_list argument_expression_list specifier_qualifier_list type_name type_specifiers
%type <node> struct_or_union struct_or_union_specifier struct_declaration_list struct_declarator_list struct_declarator struct_declaration storage_class_specifier
%type <node> type_qualifier
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
	| postfix_expression '[' expression ']' { $$ = create_array_expression_node($1, $3); }
	| postfix_expression '(' ')' { $$ = create_function_call_node($1, create_empty_statement_node()); }
	| postfix_expression '(' argument_expression_list ')' { $$ = create_function_call_node($1, $3); }
	| postfix_expression '.' IDENTIFIER { $$ = create_member_access_expression_node($1, create_identifier_node($3)); }
	| postfix_expression POINTER_OP IDENTIFIER { $$ = create_pointer__member_access_expression_node($1,create_identifier_node($3)); }
	| postfix_expression INCR_OP { $$ = create_postfix_increment_node($1); }
	| postfix_expression DECR_OP { $$ = create_postfix_decrement_node($1); }
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression {
		Node** arguments = (Node**)malloc(1 * sizeof(Node*));
		arguments[0] = $1;
		$$ = create_arguments_node(arguments, 1);
	}
	| argument_expression_list ',' assignment_expression {
		ArgumentsNode* prev_node = (ArgumentsNode*)$1;
		Node** new_arguments = (Node**)realloc(prev_node->arguments, (prev_node->count + 1) * sizeof(Node*));
		new_arguments[prev_node->count] = $3;
        
        prev_node->arguments = new_arguments;
        prev_node->count++;
        
        $$ = $1;
	}
	;

unary_expression
	: postfix_expression { $$ = $1; }
	| INCR_OP unary_expression { $$ = create_prefix_increment_node($2); }
	| DECR_OP unary_expression { $$ = create_prefix_decrement_node($2); }
	| unary_operator cast_expression { $$ = create_unary_operator_expression_node($1, $2); }
	| SIZEOF unary_expression { $$ = create_sizeof_node($2); }
	| SIZEOF '(' type_name ')' { $$ = create_sizeof_node($3); }
	;

unary_operator
	: '&' { $$ = "&"; }
	| '*' { $$ = "*"; }
	| '+' { $$ = "+"; }
	| '-' { $$ = "-"; }
	| '~' { $$ = "~"; }
	| '!' { $$ = "!"; }
	;

cast_expression
	: unary_expression { $$ = $1; }
	| '(' type_name ')' cast_expression { $$ = create_cast_expression_node($2,$4); }
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
    : declaration_specifiers ';' { $$ = $1; }
    | declaration_specifiers init_declarator_list ';' {
		DeclarationNode* decl_node = (DeclarationNode*)$2;
		decl_node->type_specifier = $1;
		$$ = $2;
		}
    ;

declaration_specifiers
	: storage_class_specifier declaration_specifiers { $$ = create_wrapper_node($1,$2); }
	| storage_class_specifier { $$ = $1; }
    | type_specifiers declaration_specifiers { $$ = create_wrapper_node($1, $2); }
    | type_specifiers { $$ = $1; }
	| type_qualifier declaration_specifiers { $$ = create_wrapper_node($1, $2); }
	| type_qualifier { $$ = $1; }
    ;

init_declarator_list
	: init_declarator { $$ = $1; }
	| init_declarator_list ',' init_declarator
	;

init_declarator
    : declarator '=' initializer { $$ = create_declaration_node($1, $3);}
	| declarator  { $$ = create_declaration_node($1, create_empty_statement_node());}
    ;

storage_class_specifier
	: TYPEDEF { $$ = create_value_node($1); }
	| EXTERN { $$ = create_value_node($1); }
	| STATIC { $$ = create_value_node($1); }
	| THREAD_LOCAL { $$ = create_value_node($1); }
	| AUTO { $$ = create_value_node($1); }
	| REGISTER { $$ = create_value_node($1); }
	;

type_specifiers
	: VOID { $$ = create_value_node($1); }
	| CHAR { $$ = create_value_node($1); }
	| SHORT { $$ = create_value_node($1); }
	| INT { $$ = create_value_node($1); }
	| LONG { $$ = create_value_node($1); }
	| FLOAT { $$ = create_value_node($1); }
	| DOUBLE { $$ = create_value_node($1); }
	| SIGNED { $$ = create_value_node($1); }
	| UNSIGNED { $$ = create_value_node($1); }
	| BOOL { $$ = create_value_node($1); }
	| COMPLEX { $$ = create_value_node($1); }
	| IMAGINARY { $$ = create_value_node($1);}
	| struct_or_union_specifier { $$ = $1; }
	| TYPEDEF_NAME { $$ = create_value_node($1);}	
    ;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}' { $$ = create_structunion_node($1,create_empty_statement_node(), $3); }
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}' { $$ = create_structunion_node($1, create_identifier_node($2), $4); }
	| struct_or_union IDENTIFIER { create_structunion_node($1,create_identifier_node($2), create_empty_statement_node()); }
	;

struct_or_union
	: STRUCT { $$ = create_value_node($1); }
	| UNION { $$ = create_value_node($1); }
	;

struct_declaration_list
	: struct_declaration {
		Node** declarations = (Node**)malloc(1 * sizeof(Node*));
		declarations[0] = $1;
		$$ = create_struct_declarations_list_node(declarations,1);
	}
	| struct_declaration_list struct_declaration {
		StructDeclarationsListNode* prev_node = (StructDeclarationsListNode*)$1;
		Node** new_declarations = (Node**)realloc(prev_node->declarations_list, (prev_node->count + 1) * sizeof(Node*));
		new_declarations[prev_node->count] = $2;
        prev_node->declarations_list = new_declarations;
        prev_node->count++;
        $$ = $1;
	}
	;

struct_declaration
	: specifier_qualifier_list ';'
	| specifier_qualifier_list struct_declarator_list ';' {
		StructDeclaratorNode* node = (StructDeclaratorNode*)$2;
		node->type = $1;
		$$ = $2;
	}
	| static_assert_declaration
	;

specifier_qualifier_list
	: type_specifiers specifier_qualifier_list
	| type_specifiers { $$ = $1; }
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator 
	;

struct_declarator
	: ':' constant_expression { $$ = create_struct_declarator_node(create_empty_statement_node(),$2);}
	| declarator ':' constant_expression { $$ = create_struct_declarator_node($1, $3); }
	| declarator { $$ = create_struct_declarator_node($1,create_empty_statement_node()); }
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
	: CONST { $$ = create_value_node($1); }
	| RESTRICT { $$ = create_value_node($1); }
	| VOLATILE { $$ = create_value_node($1); }
	| ATOMIC { $$ = create_value_node($1); }
	;

declarator
    : pointer direct_declarator { $$ = create_pointer_node($2); }
    | direct_declarator { $$ = $1;}
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
	| direct_declarator '[' assignment_expression ']' { $$ = create_array_declaration_node($1,$3); }
    | direct_declarator '(' parameter_type_list ')' { $$ = create_function_declarator_node($1, $3); }
	| direct_declarator '(' ')' { $$ = create_function_declarator_node($1, create_empty_statement_node());}
    | direct_declarator '(' identifier_list ')'
	;

pointer
    : '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer
	| '*'
	;


type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;

parameter_type_list
	: parameter_list ',' ELLIPSIS
	| parameter_list { $$ = $1; }
	;

parameter_list
	: parameter_declaration { 
		Node** parameters = (Node**)malloc(1 * sizeof(Node*));
		parameters[0] = $1;
		$$ = create_parameters_node(parameters,1);
	}
	| parameter_list ',' parameter_declaration {
		ParametersNode* prev_node = (ParametersNode*)$1;
		Node** new_parameters = (Node**)realloc(prev_node->parameters, (prev_node->count + 1) * sizeof(Node*));
		new_parameters[prev_node->count] = $3;
        
        prev_node->parameters = new_parameters;
        prev_node->count++;
        
        $$ = $1;
	}
	;

parameter_declaration
	: declaration_specifiers declarator { 
		Node* node = create_declaration_node($2,create_empty_statement_node()); 
		DeclarationNode* decl_node = (DeclarationNode*)node;
		decl_node->type_specifier = $1;
		
		$$ = (Node*)decl_node;
	}
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list { $$ = $1; }
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
	: IDENTIFIER ':' statement { $$ = create_labeled_statement_node($1,$3); }
	| CASE constant_expression ':' statement { $$ = create_case_node($2, $4); }
	| DEFAULT ':' statement { $$ = create_default_node($3); }
	;

compound_statement
	: '{' '}' {
		Node** statements = (Node**)malloc(1 * sizeof(Node*));
		statements[0] = create_empty_statement_node();
		$$ = create_compound_statement_node(statements,1);
	}
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
	| IF '(' expression ')' statement  { $$ = create_if_node($3,$5,create_empty_statement_node());}
	| SWITCH '(' expression ')' statement { $$ = create_switch_node($3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = create_while_node($3,$5);}
	| DO statement WHILE '(' expression ')' ';' { $$ = create_do_while_node($2,$5); }
	| FOR '(' expression_statement expression_statement ')' statement { $$ = create_for_node(create_empty_statement_node(), $3, $4, $6); }
	| FOR '(' expression_statement expression_statement expression ')' statement { $$ = create_for_node($3, $4, $5, $7); }
	| FOR '(' declaration expression_statement ')' statement { $$ = create_for_node($3, $4, create_empty_statement_node(), $6); }
	| FOR '(' declaration expression_statement expression ')' statement { $$ = create_for_node($3, $4, $5, $7); }

jump_statement
	: GOTO IDENTIFIER ';' {$$ = create_goto_node(create_identifier_node($2)); }
	| CONTINUE ';' { $$ = create_continue_node(); }
	| BREAK ';' { $$ = create_break_node(); }
	| RETURN ';' { $$ = create_return_node(create_empty_statement_node()); }
	| RETURN expression ';' { $$ = create_return_node($2); }
	;

program
	: external_declaration { root = $1; }
	| program external_declaration 
	;

external_declaration
	: function_definition { $$ = $1; }
	| declaration { $$ = $1; }
	;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement 
    | declaration_specifiers declarator compound_statement { $$ = create_function_declaration_node($1,$2,$3); }
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
	/* yydebug = 1; */
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