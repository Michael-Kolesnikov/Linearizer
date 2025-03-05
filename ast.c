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

Node* create_binary_operation_node(int op, Node* left, Node* right) {
    BinaryOperationNode* node = (BinaryOperationNode*)malloc(sizeof(BinaryOperationNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        exit(1);
    }
    node->base.type = BINARY_OPERATION_NODE;
    node->base.print = print_binary_operation_node;
    node->op = op;
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
    printf("Binary Operation: %c\n", bin_node->op);
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
    printf("declarator identifier: ");
    declaration_node->identifier->print(declaration_node->identifier);
    printf("declarator initializer: ");
    declaration_node->initializer->print(declaration_node->initializer);
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
    IfNode* declaration_node = (IfNode*)node;
    printf("If node Condition: ");
    declaration_node->condition->print(declaration_node->condition);
}