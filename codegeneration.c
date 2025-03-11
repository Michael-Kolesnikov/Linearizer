#include <stdio.h>
#include "ast.h"

static int label_counter = 1;

char* generate_label() {
    char* label = (char*)malloc(10 * sizeof(char));
    sprintf(label, "lbl%d", label_counter++);
    return label;
}

void generate_code_from_ast(Node* node, FILE* output) {
    if (!node) return;

    switch (node->type) {
        case IDENTIFIER_NODE: {
            IdentifierNode* id_node = (IdentifierNode*)node;
            fprintf(output, "%s", id_node->name);
            break;
        }
        case CONSTANT_NODE: {
            ConstantNode* const_node = (ConstantNode*)node;
            if (const_node->const_type == CONST_INT) {
                fprintf(output, "%d", const_node->value.i_value);
            } else if (const_node->const_type == CONST_FLOAT) {
                fprintf(output, "%f", const_node->value.f_value);
            }
            break;
        }
        case BINARY_OPERATION_NODE: {
            BinaryOperationNode* bin_node = (BinaryOperationNode*)node;
            generate_code_from_ast(bin_node->left, output);
            fprintf(output, " %c ", bin_node->op);
            generate_code_from_ast(bin_node->right, output);
            break;
        }
        case DECLARATION_NODE: {
            DeclarationNode* decl_node = (DeclarationNode*)node;
            fprintf(output, "%s ",decl_node->type_specifier);
            generate_code_from_ast(decl_node->identifier, output);
            if(decl_node->initializer != NULL){
                fprintf(output, " = ");
                generate_code_from_ast(decl_node->initializer, output);
            }
            fprintf(output, ";\n");
            break;
        }
        case ASSIGNMENT_NODE: {
            AssignmentNode* assign_node = (AssignmentNode*)node;
            generate_code_from_ast(assign_node->left, output);
            fprintf(output, " %s ", assign_node->op);
            generate_code_from_ast(assign_node->right, output);
            break;
        }
        case LOGICAL_OPERATION_NODE: {
            LogicalOperationNode* log_node = (LogicalOperationNode*)node;
            generate_code_from_ast(log_node->left, output);
            fprintf(output, " %s ", log_node->op);
            generate_code_from_ast(log_node->right, output);
            break;
        }
        case IF_NODE: {
            IfNode* if_node = (IfNode*)node;

            char* else_label = generate_label();
            char* end_label = generate_label();

            fprintf(output, "if (");
            generate_code_from_ast(if_node->condition, output);
            fprintf(output, ") goto %s;\n", else_label);

            if (if_node->else_statement) {
                generate_code_from_ast(if_node->else_statement, output);
            }
            fprintf(output, "goto %s;\n", end_label);
            fprintf(output, "%s:\n", else_label);

            generate_code_from_ast(if_node->then_statement, output);

            fprintf(output, "%s:\n", end_label);

            free(else_label);
            free(end_label);
            break;
        }
        case EXPRESSION_STATEMENT_NODE: {
            ExpressionStatementNode* expr_stmt_node = (ExpressionStatementNode*)node;
            generate_code_from_ast(expr_stmt_node->expr, output);
            fprintf(output, ";\n");
            break;
        }
        case EMPTY_STATEMENT_NODE: {
            fprintf(output, ";\n");
            break;
        }
        case COMPOUND_STATEMENT_NODE: {
            CompoundStatementNode* compound_node = (CompoundStatementNode*)node;
            for (int i = 0; i < compound_node->count; i++) {
                generate_code_from_ast(compound_node->statements[i], output);
            }
            break;
        }
        case FUNCTION_DECLARATION_NODE: {
            FunctionDeclarationNode* func_decl_node = (FunctionDeclarationNode*)node;
            fprintf(output, "%s ", func_decl_node->return_type);
            generate_code_from_ast(func_decl_node->name, output);
            fprintf(output, "() {\n");
            generate_code_from_ast(func_decl_node->body, output);
            fprintf(output, "}\n");
            break;
        }
        default:
            fprintf(output, "Unknown node type\n");
            break;
    }
}