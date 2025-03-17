#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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

Node* create_function_declaration_node(char* return_type, Node* name, Node* body){
    FunctionDeclarationNode* node = (FunctionDeclarationNode*)malloc(sizeof(FunctionDeclarationNode));
    node->base.type = FUNCTION_DECLARATION_NODE;
    node->base.print = print_function_declaration_node;
    node->name = name;
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

void print_identifier_node(Node* node){
    if (!node || node->type != IDENTIFIER_NODE) {
        printf("Invalid IdentifierNode\n");
        return;
    }
    IdentifierNode* id_node = (IdentifierNode*)node;
    printf("Identifier: %s\n",id_node->name);
}

void print_const_node(Node* node){
    if (!node || node->type != CONSTANT_NODE) {
        printf("Invalid ConstantNode\n");
        return;
    }
    ConstantNode* const_node = (ConstantNode*)node;
    if (const_node->const_type == CONST_INT) {
        printf("Constant (int): %d\n", const_node->value.i_value);
    } else if (const_node->const_type == CONST_FLOAT) {
        printf("Constant (float): %f\n", const_node->value.f_value);
    } else {
        printf("Constant: Unknown type\n");
    }
}

static char* node_type_to_string(NodeType type) {
    switch (type) {
        case IDENTIFIER_NODE: return "IDENTIFIER_NODE";
        case CONSTANT_NODE: return "CONSTANT_NODE";
        case BINARY_OPERATION_NODE: return "BINARY_OP_NODE";
        default: return "Unknown";
    }
}

void print_binary_operation_node(Node* node){
    if (!node || node->type != BINARY_OPERATION_NODE) {
        printf("Invalid BinOpNode\n");
        return;
    }
    BinaryOperationNode* bin_node = (BinaryOperationNode*)node;
    printf("Binary Operation: %s\n", bin_node->op);
    if(bin_node->left == NULL){
    }else{
        printf("Left type: %s (%d)\n", node_type_to_string(bin_node->left->type), bin_node->left->type);
        bin_node->left->print(bin_node->left);
    }
    if(bin_node->right == NULL){
    }else{
        printf("Right type: %s (%d)\n", node_type_to_string(bin_node->right->type), bin_node->right->type);
        bin_node->right->print(bin_node->right);
    }
}

void print_declaration_node(Node* node){
    if (!node || node->type != DECLARATION_NODE) {
        printf("Invalid BinOpNode\n");
        return;
    }
    DeclarationNode* declaration_node = (DeclarationNode*)node;
    printf("declorator type: %s\n", declaration_node->type_specifier);

    printf("declarator identifier: ");
    declaration_node->identifier->print(declaration_node->identifier);
    if(declaration_node->initializer == NULL){
        printf("no initializer\n");
    }else{
        printf("declarator initializer: ");
        declaration_node->initializer->print(declaration_node->initializer);
    }
}

void print_assignment_node(Node* node){
    if (!node || node->type != ASSIGNMENT_NODE) {
        printf("Invalid AssignmentNode\n");
        return;
    }
    AssignmentNode* declaration_node = (AssignmentNode*)node;
    printf("assignment left part: ");
    declaration_node->left->print(declaration_node->left);
    printf("operation: %s\n", declaration_node->op);
    printf("assignment right part: ");
    declaration_node->right->print(declaration_node->right);

}

void print_logical_operation_node(Node* node){
    if (!node || node->type != LOGICAL_OPERATION_NODE) {
        printf("Invalid LogicalNode\n");
        return;
    }
    LogicalOperationNode* declaration_node = (LogicalOperationNode*)node;
    printf("Operation %s\n", declaration_node->op);
    printf("Left: ");
    declaration_node->left->print(declaration_node->left);
    printf("Right: ");
    declaration_node->right->print(declaration_node->right);
}

void print_if_node(Node* node){
    if (!node || node->type != IF_NODE) {
        printf("Invalid LogicalNode\n");
        return;
    }
    IfNode* if_node = (IfNode*)node;
    printf("If node Condition: ");
    if_node->condition->print(if_node->condition);
    printf("IF statement: ");
    if_node->then_statement->print(if_node->then_statement);
    if(if_node->else_statement != NULL){
        printf("ELSE statement: ");
        if_node->else_statement->print(if_node->else_statement);
    }
    
}

void print_expression_statement_node(Node* node){
    if (!node || node->type != EXPRESSION_STATEMENT_NODE) {
        printf("Invalid ExpressionStatementNode\n");
        return;
    }
    ExpressionStatementNode* expression_statement_node = (ExpressionStatementNode*)node;
    printf("Expression statement: ");
    expression_statement_node->expr->print(expression_statement_node->expr);
}

void print_empty_statement_node(Node* node){
    printf("empty \t");
}
void print_compound_statement_node(Node* node){
    CompoundStatementNode* compound_node = (CompoundStatementNode*)node;
    printf("compound Node: ");
    printf("%d\n",compound_node->count);
    for(int i = 0; i < compound_node->count; i++){
        printf("[%d]: ",i);
        compound_node->statements[i]->print(compound_node->statements[i]);
    }
}

void print_function_declaration_node(Node* node){
    FunctionDeclarationNode* func_decl_node = (FunctionDeclarationNode*)node;
    printf("function type: %s\n",func_decl_node->return_type);
    printf("function name: ");
    func_decl_node->name->print(func_decl_node->name);
    printf("function body: \n");
    func_decl_node->body->print(func_decl_node->body);
}
void print_string_literal_node(Node* node){
    StringNode* string = (StringNode*)node;

    printf("string: %s",string->value);
}
void print_while_node(Node* node){
    WhileNode* while_node = (WhileNode*)node;
    printf("WHile condition: ");
    while_node->condition->print(while_node->condition);
    printf("While body: ");
    while_node->body->print(while_node->body);
}
void print_do_while_node(Node* node){
    DoWhileNode* while_node = (DoWhileNode*)node;
    printf("doWhile body: ");
    while_node->do_statement->print(while_node->do_statement);

    printf("DoWHile condition: ");
    while_node->condition->print(while_node->condition);
}

void print_switch_node(Node* node){
    SwitchNode* switch_node = (SwitchNode*)node;
    printf("SWITCH EXPRESSION: ");
    switch_node->expression->print(switch_node->expression);
    printf("switch body: ");
    switch_node->body->print(switch_node->body);
}

void print_case_node(Node* node){
    CaseNode* case_node = (CaseNode*)node;
    printf("CASE EXPRESSION: ");
    case_node->expression->print(case_node->expression);
    printf("case body: ");
    case_node->body->print(case_node->body);
}
void print_default_node(Node* node){
    DefaultNode* def_node = (DefaultNode*)node;
    printf("DEFAULT: ");
    def_node->body->print(def_node->body);
}

void print_break_node(Node* node){
    printf("break");
}

void print_ternary_operator_node(Node* node){
    TernaryOperatorNode* ternary_node = (TernaryOperatorNode*)node;
    printf("ternary Condition: ");
    ternary_node->condition->print(ternary_node->condition);
    printf("ternary then statement: ");
    ternary_node->then_statement->print(ternary_node->then_statement);
    if(ternary_node->else_statement != NULL){
        printf("ternary ELSE statement: ");
        ternary_node->else_statement->print(ternary_node->else_statement);
    }
}

void print_return_node(Node* node){
    ReturnNode* return_node = (ReturnNode*)node;
    printf("return: ");
    if(return_node->expression != NULL){
        return_node->expression->print(return_node->expression);
    }
}