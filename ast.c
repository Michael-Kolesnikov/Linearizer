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
    printf("Binary Operation: %c\n", bin_node->op);  // Выводим символ
    printf("Left: ");
    if(bin_node->left == NULL){
    }else{
        printf("Left type: %s (%d)\n", node_type_to_string(bin_node->left->type), bin_node->left->type);
        bin_node->left->print(bin_node->left);
    }
    printf("Right: ");
    if(bin_node->right == NULL){
    }else{
        printf("Right type: %s (%d)\n", node_type_to_string(bin_node->left->type), bin_node->left->type);
        bin_node->right->print(bin_node->right);
    }
}
