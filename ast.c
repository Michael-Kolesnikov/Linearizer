#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Node* create_identifier_node(char* name) {
    IdentifierNode* node = (IdentifierNode*)malloc(sizeof(IdentifierNode));
    if(!node) {
        fprintf(stderr, "Error: Failed to allocate memory for IdentifierNode\n");
        exit(1);
    }
    node->base.type = IDENTIFIER_NODE;
    node->name = strdup(name);
    if (!node->name) {
        fprintf(stderr, "Error: Failed to duplicate string\n");
        free(node);
        exit(1);
    }
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
}

void print_identifier(Node* node) {
    if (!node || node->type != IDENTIFIER_NODE) {
        printf("Invalid IdentifierNode\n");
        return;
    }
    IdentifierNode* idNode = (IdentifierNode*)node;
    printf("Identifier: %s\n",idNode->name);
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