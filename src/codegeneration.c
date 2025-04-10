#include <stdio.h>
#include <stdlib.h>
#include "src/ast.h"


typedef enum {
    CONTEXT_DEFAULT,
    CONTEXT_PARAMETERS,
    CONTEXT_COMPOUND,
    CONTEXT_FOR,
} Context;

static int label_counter = 1;
static FILE* output_file;
static Context current_context = CONTEXT_DEFAULT;

char* generate_label() {
    char* label = (char*)malloc(10 * sizeof(char));
    sprintf(label, "lbl%d", label_counter++);
    return label;
}

void generate_code(Node* node){

    switch (node->type) {
        case IF_NODE: {
            IfNode* if_node = (IfNode*)node;
            char* else_label = generate_label();
            char* end_label = generate_label();
            fprintf(output_file, "if (");
            generate_code(if_node->condition);
            fprintf(output_file, ") goto %s;\n", else_label);
            if (if_node->else_statement) {
                generate_code(if_node->else_statement);
            }
            fprintf(output_file, "goto %s;\n", end_label);
            fprintf(output_file, "%s:\n", else_label);
            generate_code(if_node->then_statement);
            fprintf(output_file, "%s:\n", end_label);
            free(else_label);
            free(end_label);
            break;
        }
        case WHILE_NODE: {
            WhileNode* while_node = (WhileNode*)node;
            char* loop_label = generate_label();
            char* end_label = generate_label();
            fprintf(output_file, "%s:\n", loop_label);
            fprintf(output_file, "if(!(");
            generate_code(while_node->condition);
            fprintf(output_file, " )) goto %s", end_label);
            fprintf(output_file, ";\n");
            generate_code(while_node->body);
            fprintf(output_file, " goto %s;\n", loop_label);
            fprintf(output_file, "%s:\n", end_label);
            break;
        }
        case FOR_NODE: {
            ForNode* for_node = (ForNode*)node;
            char* loop_label = generate_label();
            char* end_label = generate_label();
            current_context = CONTEXT_FOR;

            if (for_node->initialization->type != EMPTY_STATEMENT_NODE) {
                current_context = CONTEXT_DEFAULT;
                generate_code(for_node->initialization);
                current_context = CONTEXT_FOR;
                fprintf(output_file, "\n");
            }
            
            fprintf(output_file, "%s:\n", loop_label);
            
            if (((ExpressionStatementNode*)for_node->condition)->expr->type != EMPTY_STATEMENT_NODE) {
                fprintf(output_file, "if (!(");
                generate_code(for_node->condition);
                fprintf(output_file, ")) goto %s;\n", end_label);
            }
            current_context = CONTEXT_DEFAULT;
            generate_code(for_node->body);
            current_context = CONTEXT_FOR;
            if (((ExpressionStatementNode*)for_node->update)->expr->type != EMPTY_STATEMENT_NODE) {
                generate_code(for_node->update);
                fprintf(output_file, ";\n");
            }
            fprintf(output_file, "goto %s;\n", loop_label);
            fprintf(output_file, "%s:", end_label);
            free(loop_label);
            free(end_label);
            current_context = CONTEXT_DEFAULT;
            break;
        }
        case ARRAY_DECLARATION_NODE: {
            ArrayDeclarationNode* array_node = (ArrayDeclarationNode*)node;
            generate_code(array_node->declarator);
            fprintf(output_file,"[");
            generate_code(array_node->index_expression);
            fprintf(output_file,"]");
            break;
        }
        case UNARY_OPERATOR_EXPRESSION_NODE: {
            UnaryOperatorExpressonNode* unary_node = (UnaryOperatorExpressonNode*)node;
            fprintf(output_file, "%s", unary_node->unary_operator);
            generate_code(unary_node->expression);
            break;
        }
        case FUNCTION_CALL_NODE: {
            FunctionCallNode* func_call_node = (FunctionCallNode*)node;
            generate_code(func_call_node->name);
            fprintf(output_file,"(");
            generate_code(func_call_node->arguments);
            fprintf(output_file,")");
            break;
        }
        case ARGUMENTS_NODE: {
            ArgumentsNode* arguments = (ArgumentsNode*)node;
            for(int i = 0 ; i < arguments->count; i++){
                generate_code(arguments->arguments[i]);
                if(arguments->count != 1 && i != arguments->count - 1){
                    fprintf(output_file,", ");
                }
            }
            break;
        }
        case ASSIGNMENT_NODE: {
            AssignmentNode* assignment_node = (AssignmentNode*)node;
            generate_code(assignment_node->left);
            fprintf(output_file," %s ",assignment_node->op);
            generate_code(assignment_node->right);
            break;
        }
        case POSTFIX_INCEMENT_NODE: {
            PostfixIncrement* incr = (PostfixIncrement*)node;
            generate_code(incr->expression);
            fprintf(output_file, "++");
            break;
        }
        case EXPRESSION_STATEMENT_NODE: {
            ExpressionStatementNode* expr_statement = (ExpressionStatementNode*)node;
            generate_code(expr_statement->expr);
            if(current_context == CONTEXT_DEFAULT && expr_statement->expr->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file,";\n");
            }
            break;
        }
        case LOGICAL_OPERATION_NODE: {
            LogicalOperationNode* logical_op = (LogicalOperationNode*)node;
            generate_code(logical_op->left);
            fprintf(output_file, " %s ", logical_op->op);
            generate_code(logical_op->right);
            break;
        }
        case FUNCTION_DECLARATION_NODE: {
            FunctionDeclarationNode* function_declaration = (FunctionDeclarationNode*)node;
            generate_code(function_declaration->return_type);
            fprintf(output_file, " ");
            generate_code(function_declaration->declarator);
            fprintf(output_file,"{\n");
            generate_code(function_declaration->body);
            fprintf(output_file,"}\n");
            break;
        }
        case VALUE_NODE: {
            ValueNode* value = (ValueNode*)node;
            fprintf(output_file,"%s",value->value);
            break;
        }
        case FUNCTION_DECLARATOR_NODE: {
            FunctionDeclaratorNode* function_declarator = (FunctionDeclaratorNode*)node;
            generate_code(function_declarator->declarator);
            fprintf(output_file, "(");
            current_context = CONTEXT_PARAMETERS;
            generate_code(function_declarator->parameters);
            current_context = CONTEXT_DEFAULT;
            fprintf(output_file, ")");
            break;
        }
        case PARAMETERS_NODE: {
            ParametersNode* params = (ParametersNode*)node;
            for(int i = 0 ; i < params->count; i++){
                generate_code(params->parameters[i]);
                if(params->count != 1 && i != params->count - 1){
                    fprintf(output_file,", ");
                }
            }
            break;
        }
        case DECLARATORS_LIST_NODE: {
            DeclaratorsListNode* decl_list = (DeclaratorsListNode*)node;
            generate_code(decl_list->type_specifier);
            for(int i = 0; i < decl_list->count; i++){
                generate_code(decl_list->declarators[i]);
                if(i != decl_list->count - 1 && i != 0){
                    fprintf(output_file, ", ");
                }
            }
            if (current_context == CONTEXT_DEFAULT) {
                fprintf(output_file, ";");
            }
            break;
        }
        case DECLARATION_NODE: {
            DeclarationNode* declaration = (DeclarationNode*)node;
            fprintf(output_file, " ");
            generate_code(declaration->identifier);
            if(declaration->initializer->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file," = ");
                generate_code(declaration->initializer);
            }
            break;
        }
        case COMPOUND_STATEMENT_NODE: {
            CompoundStatementNode* compound_statement = (CompoundStatementNode*)node;
            for(int i = 0 ; i < compound_statement->count; i++){
                generate_code(compound_statement->statements[i]);
                if(compound_statement->statements[i]->type != EMPTY_STATEMENT_NODE){
                    fprintf(output_file, "\n");
                }
            }
            break;
        }
        case POINTER_NODE: {
            PointerNode* pointer = (PointerNode*)node;
            fprintf(output_file, "*");
            generate_code(pointer->declarator);
            break;
        }
        case STRING_LITERAL_NODE: {
            StringNode* string = (StringNode*)node;
            fprintf(output_file,"%s",string->value);
        }
        case CONSTANT_NODE: {
            ConstantNode* constant = (ConstantNode*)node;
            if(constant->const_type == CONST_INT){
                fprintf(output_file,"%d",constant->value.i_value);
            }else if(constant->const_type == CONST_FLOAT){
                fprintf(output_file,"%f",constant->value.f_value);
            }
            break;
        }
        case EMPTY_STATEMENT_NODE: {
            break;
        }
        case IDENTIFIER_NODE: {
            IdentifierNode* id_node = (IdentifierNode*)node;
            fprintf(output_file, "%s", id_node->name);
            break;
        }
        default: {
            break;
        }
    }
}

void generate_code_from_ast(Node* root, FILE* output) {
    if (!root) return;
    label_counter = 1;
    output_file = output;
    generate_code(root);
}
