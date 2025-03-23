#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED "\x1b[41m"
#define COLOR_YELLOW "\x1b[33m"
static int indent_level = 0;

void print_indent() {
    for (int i = 0; i < indent_level; i++) {
        printf("  ");
    }
}

Node* create_identifier_node(char* name){
    IdentifierNode* node = (IdentifierNode*)malloc(sizeof(IdentifierNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for IdentifierNode\n");
        exit(1);
    }
    node->name = strdup(name);
    if (!node->name) {
        fprintf(stderr, "Error: Failed to duplicate string\n");
        free(node);
        exit(1);
    }
    node->base.type = IDENTIFIER_NODE;
    node->base.print = print_identifier_node;
    return (Node*)node;
}

Node* create_constant_int_node(int value) {
    ConstantNode* node = (ConstantNode*)malloc(sizeof(ConstantNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for ConstantNode\n");
        exit(1);
    }
    node->base.type = CONSTANT_NODE;
    node->const_type = CONST_INT;
    node->value.i_value = value;
    node->base.print = print_const_node;
    return (Node*)node;
}

Node* create_constant_float_node(float value){
    ConstantNode* node = (ConstantNode*)malloc(sizeof(ConstantNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for ConstantNode\n");
        exit(1);
    }
    node->base.type = CONSTANT_NODE;
    node->const_type = CONST_FLOAT;
    node->value.f_value = value;
    node->base.print = print_const_node;
    return (Node*)node;
}

Node* create_binary_operation_node(char* op, Node* left, Node* right) {
    BinaryOperationNode* node = (BinaryOperationNode*)malloc(sizeof(BinaryOperationNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        exit(1);
    }
    node->base.type = BINARY_OPERATION_NODE;
    node->base.print = print_binary_operation_node;
    node->op = strdup(op);
    node->left = left;
    node->right = right;
    return (Node*)node;
}

Node* create_declaration_node(Node* identifier, Node* initializer){
    DeclarationNode* node = (DeclarationNode*)malloc(sizeof(DeclarationNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for DeclarationNode\n");
        exit(1);
    }

    node->base.type = DECLARATION_NODE;
    node->base.print = print_declaration_node;
    node->identifier = identifier;
    node->initializer = initializer;

    return (Node*)node;
}

Node* create_assignment_node(Node* left, char* op, Node* right){
    AssignmentNode* node = (AssignmentNode*)malloc(sizeof(AssignmentNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for AssignmentNode\n");
        exit(1);
    }

    node->base.type = ASSIGNMENT_NODE;
    node->base.print = print_assignment_node;
    node->left = left;
    node->op = op;
    node->right = right;

    return (Node*)node;
}

Node* create_logical_operation_node(Node* left, char* op, Node* right){
    LogicalOperationNode* node = (LogicalOperationNode*)malloc(sizeof(LogicalOperationNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for LogicalOperationNode\n");
        exit(1);
    }
    node->base.type = LOGICAL_OPERATION_NODE;
    node->base.print = print_logical_operation_node;
    node->left = left;
    node->op = strdup(op);
    if (!node->op) {
        fprintf(stderr, "Error: Failed to duplicate operator string\n");
        free(node);
        exit(1);
    }
    node->right = right;
    return (Node*)node;
}

Node* create_if_node(Node* condition, Node* then_statement, Node* else_statement){
    IfNode* node = (IfNode*)malloc(sizeof(IfNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for IfNode\n");
        exit(1);
    }
    
    node->base.type = IF_NODE;
    node->base.print = print_if_node;
    node->condition = condition;
    node->then_statement = then_statement;
    node->else_statement = else_statement;
}

Node* create_expression_statement_node(Node* expr){
    ExpressionStatementNode* node = (ExpressionStatementNode*)malloc(sizeof(ExpressionStatementNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for ExpressionStatementNode\n");
        exit(1);
    }

    node->base.type = EXPRESSION_STATEMENT_NODE;
    node->base.print = print_expression_statement_node;
    node->expr = expr;
    return (Node*)node;
}

Node* create_empty_statement_node(){
    EmptyStatementNode* node = (EmptyStatementNode*)malloc(sizeof(EmptyStatementNode));
    if(!node) {
        fprintf(stderr, "Error: Failed to allocate memory for EmptyStatementNode\n");
        exit(1);
    }

    node->base.type = EMPTY_STATEMENT_NODE;
    node->base.print = print_empty_statement_node;
    return (Node*)node;
}

Node* create_compound_statement_node(Node** statement, int count){
    CompoundStatementNode* node = (CompoundStatementNode*)malloc(sizeof(CompoundStatementNode));
    
    node->statements = statement;
    node->count = count;
    node->base.type = COMPOUND_STATEMENT_NODE;
    node->base.print= print_compound_statement_node;
    return (Node*)node;
}

Node* create_function_declaration_node(char* return_type, Node* declarator, Node* body){
    FunctionDeclarationNode* node = (FunctionDeclarationNode*)malloc(sizeof(FunctionDeclarationNode));
    node->base.type = FUNCTION_DECLARATION_NODE;
    node->base.print = print_function_declaration_node;
    node->declarator = declarator;
    node->return_type = strdup(return_type);
    node->body = body;
}

Node* create_string_literal_node(char* value){
    StringNode* node = (StringNode*)malloc(sizeof(StringNode));
    node->base.type = STRING_LITERAL_NODE;
    node->base.print = print_string_literal_node;
    node->value = strdup(value);
}

Node* create_while_node(Node* condition, Node* body){
    WhileNode* node = (WhileNode*)malloc(sizeof(WhileNode));
    node->base.type = WHILE_NODE;
    node->base.print = print_while_node;
    node->condition = condition;
    node->body = body;
}

Node* create_do_while_node(Node* do_statement, Node* condition){
    DoWhileNode* node = (DoWhileNode*)malloc(sizeof(WhileNode));
    node->base.type = DOWHILE_NODE;
    node->base.print = print_do_while_node;
    node->do_statement = do_statement;
    node->condition = condition;
}

Node* create_switch_node(Node* expression, Node* body){
    SwitchNode* node = (SwitchNode*)malloc(sizeof(SwitchNode));
    node->base.type = SWITCH_NODE;
    node->base.print = print_switch_node;
    node->expression = expression;
    node->body = body;
}

Node* create_case_node(Node* expression, Node* body){
    CaseNode* node = (CaseNode*)malloc(sizeof(CaseNode));
    node->base.type = CASE_NODE;
    node->base.print = print_case_node;
    node->expression = expression;
    node->body = body;
}

Node* create_default_node(Node* body){
    DefaultNode* node = (DefaultNode*)malloc(sizeof(DefaultNode));
    node->base.type = DEFAULT_NODE;
    node->base.print = print_default_node;
    node->body = body;
    return (Node*)node;
}

Node* create_break_node(){
    BreakNode* node = (BreakNode*)malloc(sizeof(BreakNode));
    node->base.type = BREAK_NODE;
    node->base.print = print_break_node;
    return (Node*)node;
}

Node* create_ternary_operator_node(Node* condition, Node* then_statement, Node* else_statement){
    TernaryOperatorNode* node = (TernaryOperatorNode*)malloc(sizeof(TernaryOperatorNode));
    node->base.type = TERNARY_OPERATOR_NODE;
    node->base.print = print_ternary_operator_node;
    node->condition = condition;
    node->then_statement = then_statement;
    node->else_statement = else_statement;
}

Node* create_return_node(Node* expression){
    ReturnNode* node = (ReturnNode*)malloc(sizeof(ReturnNode));
    node->base.type = RETURN_NODE;
    node->base.print = print_return_node;
    node->expression = expression;
}

Node* create_goto_node(Node* identifier){
    GotoNode* node = (GotoNode*)malloc(sizeof(GotoNode));
    node->base.type = GOTO_NODE;
    node->base.print = print_goto_node;
    node->identifier = identifier;
}

Node* create_continue_node(){
    ContinueNode* node = (ContinueNode*)malloc(sizeof(ContinueNode));
    node->base.type = CONTINUE_NODE;
    node->base.print = print_continue_node;
}

Node* create_prefix_increment_node(Node* expression){
    PrefixIncrement* node = (PrefixIncrement*)malloc(sizeof(PrefixIncrement));
    node->base.type = PREFIX_INCREMENT_NODE;
    node->base.print = print_prefix_increment_node;
    node->expression = expression;
}

Node* create_postfix_increment_node(Node* expression){
    PostfixIncrement* node = (PostfixIncrement*)malloc(sizeof(PostfixIncrement));
    node->base.type = POSTFIX_INCEMENT_NODE;
    node->base.print = print_postfix_increment_node;
    node->expression = expression;
}

Node* create_prefix_decrement_node(Node* expression){
    PrefixDecrement* node = (PrefixDecrement*)malloc(sizeof(PrefixDecrement));
    node->base.type = PREFIX_DECREMENT_NODE;
    node->base.print = print_prefix_decrement_node;
    node->expression = expression;
}

Node* create_postfix_decrement_node(Node* expression){
    PostfixDecrement* node = (PostfixDecrement*)malloc(sizeof(PostfixDecrement));
    node->base.type = POSTFIX_DECREMENT_NODE;
    node->base.print = print_postfix_decrement_node;
    node->expression = expression;
}

Node* create_unary_operator_expression_node(char* unary_operator, Node* expression){
    UnaryOperatorExpressonNode* node = (UnaryOperatorExpressonNode*)malloc(sizeof(UnaryOperatorExpressonNode));
    node->base.type = UNARY_OPERATOR_EXPRESSION_NODE;
    node->base.print = print_unary_operator_expression_node;
    node->unary_operator = strdup(unary_operator);
    node->expression = expression;
}

Node* create_pointer_node(Node* declarator){
    PointerNode* node = (PointerNode*)malloc(sizeof(PointerNode));
    node->base.type = POINTER_NODE;
    node->base.print = print_pointer_node;
    node->declarator = declarator;
}

Node* create_parameters_node(Node** parameters, int count){
    ParametersNode* node = (ParametersNode*)malloc(sizeof(ParametersNode));
    
    node->parameters = parameters;
    node->count = count;
    node->base.type = PARAMETERS_NODE;
    node->base.print= print_parameters_node;
    return (Node*)node;
}

Node* create_function_declarator_node(Node* declarator, Node* parameters){
    FunctionDeclaratorNode* node = (FunctionDeclaratorNode*)malloc(sizeof(FunctionDeclaratorNode));
    node->declarator = declarator;
    node->parameters = parameters;
    node->base.print = print_function_declarator_node;
    node->base.type = FUNCTION_DECLARATOR_NODE;
    return (Node*)node;
}

Node* create_for_node(Node* initialization, Node* condition, Node* update, Node* body){
    ForNode* node = (ForNode*)malloc(sizeof(ForNode));
    node->base.type = FOR_NODE;
    node->base.print = print_for_node;
    node->initialization = initialization;
    node->condition = condition;
    node->update = update;
    node->body = body;
}

void print_identifier_node(Node* node){
    IdentifierNode* id_node = (IdentifierNode*)node;
    print_indent();
    printf(COLOR_BLUE "Identifier Node: " COLOR_GREEN "%s" COLOR_RESET "\n", id_node->name);
}

void print_const_node(Node* node){
    ConstantNode* const_node = (ConstantNode*)node;
    print_indent();
    if (const_node->const_type == CONST_INT) {
        printf(COLOR_BLUE "Constant (int) Node: " COLOR_GREEN "%d\n" COLOR_RESET, const_node->value.i_value);
    } else if (const_node->const_type == CONST_FLOAT) {
        printf(COLOR_BLUE "Constant (float) Node: " COLOR_GREEN " %f\n" COLOR_RESET, const_node->value.f_value);
    } else {
        printf(COLOR_RED "Constant: Unknown type" COLOR_RESET "\n");
    }
}

void print_binary_operation_node(Node* node){
    BinaryOperationNode* bin_node = (BinaryOperationNode*)node;
    printf(COLOR_BLUE "Binary operation Node: " COLOR_YELLOW "%s" COLOR_RESET "\n", bin_node->op);
    print_indent();
    if(bin_node->left == NULL){
    }else{
        printf("Left:\n", bin_node->op);
        indent_level++;
        bin_node->left->print(bin_node->left);
        indent_level--;
    }

    if(bin_node->right == NULL){
    }else{
        printf("Right:\n", bin_node->op);
        indent_level++;
        bin_node->right->print(bin_node->right);
        indent_level--;
    }
}

void print_declaration_node(Node* node){
    DeclarationNode* declaration_node = (DeclarationNode*)node;
    print_indent();
    printf( COLOR_BLUE "Declaration Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Declarator type: " COLOR_GREEN "%s" COLOR_RESET "\n", declaration_node->type_specifier);
    print_indent();
    printf("Declarator identifier:\n");
    indent_level++;
    declaration_node->identifier->print(declaration_node->identifier);
    indent_level--;
    if(declaration_node->initializer->type != EMPTY_STATEMENT_NODE){
        print_indent();
        printf("declarator initializer: \n");
        indent_level++;
        declaration_node->initializer->print(declaration_node->initializer);
        indent_level--;
    }
    indent_level--;
}

void print_assignment_node(Node* node){
    AssignmentNode* declaration_node = (AssignmentNode*)node;
    print_indent();
    printf(COLOR_BLUE "Assignment Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Operation: " COLOR_YELLOW "%s" COLOR_RESET "\n",declaration_node->op);
    print_indent();
    printf("Left: \n");
    indent_level++;
    declaration_node->left->print(declaration_node->left);
    indent_level--;
    print_indent();
    printf("Right: \n");
    indent_level++;
    declaration_node->right->print(declaration_node->right);
    indent_level--;
    indent_level--;
}

void print_logical_operation_node(Node* node){
    LogicalOperationNode* declaration_node = (LogicalOperationNode*)node;
    print_indent();
    printf(COLOR_BLUE "Logical operation Node:"COLOR_RESET"\n");
    indent_level++;
    print_indent();
    printf("Operation: " COLOR_YELLOW "%s" COLOR_RESET "\n", declaration_node->op);
    print_indent();
    printf("Left: \n");
    indent_level++;
    declaration_node->left->print(declaration_node->left);
    indent_level--;
    print_indent();
    printf("Right: \n");
    indent_level++;
    declaration_node->right->print(declaration_node->right);
    indent_level--;
    indent_level--;
}

void print_if_node(Node* node){
    IfNode* if_node = (IfNode*)node;
    print_indent();
    printf(COLOR_BLUE "If Node" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("If Condition: \n");
    indent_level++;
    if_node->condition->print(if_node->condition);
    indent_level--;
    print_indent();
    printf("Then: \n");
    indent_level++;
    if_node->then_statement->print(if_node->then_statement);
    indent_level--;
    if(if_node->else_statement != NULL){
        print_indent();
        printf("Else: \n");
        indent_level++;
        if_node->else_statement->print(if_node->else_statement);
        indent_level--;
    }
}

void print_expression_statement_node(Node* node){
    ExpressionStatementNode* expression_statement_node = (ExpressionStatementNode*)node;
    print_indent();
    printf(COLOR_BLUE "Expression statement Node: " COLOR_RESET "\n");
    indent_level++;
    expression_statement_node->expr->print(expression_statement_node->expr);
    indent_level--;
}

void print_empty_statement_node(Node* node){
    print_indent();
    printf(COLOR_BLUE "Empty statement: " COLOR_GREEN "\" \"" COLOR_RESET "\n");
}

void print_compound_statement_node(Node* node){
    CompoundStatementNode* compound_node = (CompoundStatementNode*)node;
    print_indent();
    printf(COLOR_BLUE "Compounds statements Node, count: %d" COLOR_RESET "\n",compound_node->count);
    indent_level++;
    for(int i = 0; i < compound_node->count; i++){
        print_indent();
        printf("statement[%d]: \n",i);
        indent_level++;
        compound_node->statements[i]->print(compound_node->statements[i]);
        indent_level--;
    }
    indent_level--;
}

void print_function_declaration_node(Node* node){
    FunctionDeclarationNode* func_decl_node = (FunctionDeclarationNode*)node;
    print_indent();
    printf(COLOR_BLUE "Function declaration Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Function type: " COLOR_GREEN "%s\n" COLOR_RESET,func_decl_node->return_type);
    print_indent();
    printf("Function declarator: \n");
    indent_level++;
    func_decl_node->declarator->print(func_decl_node->declarator);
    indent_level--;
    print_indent();
    printf("Function body: \n");
    indent_level++;
    func_decl_node->body->print(func_decl_node->body);
    indent_level--;
    indent_level--;
}

void print_string_literal_node(Node* node){
    StringNode* string = (StringNode*)node;
    print_indent();
    printf(COLOR_BLUE "String Node: " COLOR_GREEN "%s" COLOR_RESET "\n",string->value);
}

void print_while_node(Node* node){
    WhileNode* while_node = (WhileNode*)node;
    print_indent();
    printf(COLOR_BLUE "While Node" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("While condition: \n");
    indent_level++;
    while_node->condition->print(while_node->condition);
    indent_level--;
    print_indent();
    printf("While body: \n");
    indent_level++;
    while_node->body->print(while_node->body);
    indent_level--;
    indent_level--;
}

void print_do_while_node(Node* node){
    DoWhileNode* while_node = (DoWhileNode*)node;
    print_indent();
    printf(COLOR_BLUE "DoWhile Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("DoWhile body: \n");
    indent_level++;
    while_node->do_statement->print(while_node->do_statement);
    indent_level--;
    print_indent();
    printf("DoWhile condition: \n");
    indent_level++;
    while_node->condition->print(while_node->condition);
    indent_level--;
}

void print_switch_node(Node* node){
    SwitchNode* switch_node = (SwitchNode*)node;
    print_indent();
    printf(COLOR_BLUE "Switch Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Switch expression: \n");
    indent_level++;
    switch_node->expression->print(switch_node->expression);
    indent_level--;
    print_indent();
    printf("Switch body: \n");
    indent_level++;
    switch_node->body->print(switch_node->body);
    indent_level--;
    indent_level--;
}

void print_case_node(Node* node){
    CaseNode* case_node = (CaseNode*)node;
    print_indent();
    printf(COLOR_BLUE "Case Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Case expression: \n");
    indent_level++;
    case_node->expression->print(case_node->expression);
    indent_level--;
    print_indent();
    printf("Case body: \n");
    indent_level++;
    case_node->body->print(case_node->body);
    indent_level--;
    indent_level--;
}

void print_default_node(Node* node){
    DefaultNode* def_node = (DefaultNode*)node;
    print_indent();
    printf(COLOR_BLUE "Default Node: " COLOR_RESET " \n");
    indent_level++;
    print_indent();
    printf("Default body: \n");
    indent_level++;
    def_node->body->print(def_node->body);
    indent_level--;
    indent_level--;
}

void print_break_node(Node* node){
    print_indent();
    printf(COLOR_BLUE "Break Node: " COLOR_RESET);
    printf(COLOR_GREEN "break" COLOR_RESET "\n");
}

void print_ternary_operator_node(Node* node){
    TernaryOperatorNode* ternary_node = (TernaryOperatorNode*)node;
    print_indent();
    printf(COLOR_BLUE "Ternary operator Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Ternary condition: \n");
    indent_level++;
    ternary_node->condition->print(ternary_node->condition);
    indent_level--;
    print_indent();
    printf("Ternary then: \n");
    indent_level++;
    ternary_node->then_statement->print(ternary_node->then_statement);
    indent_level--;
    if(ternary_node->else_statement != NULL){
        print_indent();
        printf("Ternary else: \n");
        indent_level++;
        ternary_node->else_statement->print(ternary_node->else_statement);
        indent_level--;
    }
    indent_level--;
}

void print_return_node(Node* node){
    ReturnNode* return_node = (ReturnNode*)node;
    print_indent();
    printf(COLOR_BLUE "Return Node: " COLOR_RESET "\n");
    if(return_node->expression != NULL){
        indent_level++;
        return_node->expression->print(return_node->expression);
        indent_level--;
    }
}

void print_goto_node(Node* node){
    GotoNode* goto_node = (GotoNode*)node;
    print_indent();
    printf(COLOR_BLUE "Goto Node: " COLOR_RESET "\n");
    indent_level++;
    goto_node->identifier->print(goto_node->identifier);
    indent_level--;
}

void print_continue_node(Node* node){
    print_indent();
    printf(COLOR_BLUE "Continue Node: " COLOR_RESET);
    printf(COLOR_GREEN "continue" COLOR_RESET "\n");
}

void print_prefix_increment_node(Node* node){
    PrefixIncrement* incr_node = (PrefixDecrement*)node;
    print_indent();
    printf(COLOR_BLUE "Prefix increment Node: " COLOR_RESET "\n");
    indent_level++;
    incr_node->expression->print(incr_node->expression);
    indent_level--;
}

void print_postfix_increment_node(Node* node){
    PostfixIncrement* incr_node = (PostfixIncrement*)node;
    print_indent();
    printf(COLOR_BLUE "Postfix increment Node: " COLOR_RESET "\n");
    indent_level++;
    incr_node->expression->print(incr_node->expression);
    indent_level--;
}

void print_prefix_decrement_node(Node* node){
    PrefixDecrement* dec_node = (PrefixDecrement*)node;
    print_indent();
    printf(COLOR_BLUE "Prefix decrement Node: " COLOR_RESET "\n");
    indent_level++;
    dec_node->expression->print(dec_node->expression);
    indent_level--;
}

void print_postfix_decrement_node(Node* node){
    PostfixDecrement* dec_node = (PostfixDecrement*)node;
    print_indent();
    printf(COLOR_BLUE "Postfix decrement Node: " COLOR_RESET "\n");
    indent_level++;
    dec_node->expression->print(dec_node->expression);
    indent_level--;
}

void print_unary_operator_expression_node(Node* node){
    UnaryOperatorExpressonNode* unar_node = (UnaryOperatorExpressonNode*)node;
    print_indent();
    printf(COLOR_BLUE "Unary operator Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Operation: " COLOR_YELLOW "%s" COLOR_RESET "\n", unar_node->unary_operator);
    print_indent();
    printf("Expression: \n");
    indent_level++;
    unar_node->expression->print(unar_node->expression);
    indent_level--;
}

void print_pointer_node(Node* node){
    PointerNode* pointer_node = (PointerNode*)node;
    print_indent();
    printf(COLOR_BLUE "Pointer Node: " COLOR_RESET " \n");
    indent_level++;
    print_indent();
    printf("Pointer declarator: \n");
    indent_level++;
    pointer_node->declarator->print(pointer_node->declarator);
    indent_level--;
    indent_level--;
}

void print_parameters_node(Node* node){
    ParametersNode* params_node = (ParametersNode*)node;
    print_indent();
    printf(COLOR_BLUE "Parameters Node, count: %d " COLOR_RESET "\n", params_node->count);
    indent_level++;
    for(int i = 0; i < params_node->count; i++){
        print_indent();
        printf("Parameter [%d]: \n",i);
        indent_level++;
        params_node->parameters[i]->print(params_node->parameters[i]);
        indent_level--;
    }
}

void print_function_declarator_node(Node* node){
    FunctionDeclaratorNode* func_decl_node = (FunctionDeclaratorNode*)node;
    print_indent();
    printf(COLOR_BLUE "Function declarator Node: " COLOR_RESET "\n");
    indent_level++;
    func_decl_node->declarator->print(func_decl_node->declarator);
    indent_level--;
    print_indent();
    printf("Function parameters: \n");
    indent_level++;
    func_decl_node->parameters->print(func_decl_node->parameters);
    indent_level--;
}

void print_for_node(Node* node){
    ForNode* for_node = (ForNode*)node;
    print_indent();
    printf(COLOR_BLUE "For Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("For initialization: \n");
    indent_level++;
    for_node->initialization->print(for_node->initialization);
    indent_level--;
    print_indent();
    printf("For condition: \n");
    indent_level++;
    for_node->condition->print(for_node->condition);
    indent_level--;
    print_indent();
    printf("For update: \n");
    indent_level++;
    for_node->update->print(for_node->update);
    indent_level--;
    print_indent();
    printf("For body: \n");
    indent_level++;
    for_node->body->print(for_node->body);
    indent_level--;
    indent_level--;
}