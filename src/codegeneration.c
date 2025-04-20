#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/ast.h"
#include "src/symbolTable.h"

typedef enum {
    CONTEXT_DEFAULT,
    CONTEXT_PARAMETERS,
    CONTEXT_COMPOUND,
    CONTEXT_FOR,
    CONTEXT_WHILE,
    CONTEXT_POINTER,
    CONTEXT_DECLARATORS_LIST,
    CONTEXT_ARRAY_INDEX,
} Context;

static int label_counter = 1;
static int temp_var_counter = 1;
static char* current_loop_end_label = "";
static FILE* output_file;
static Context current_context = CONTEXT_DEFAULT;
static int inLoop = 0;

char* generate_label() {
    char* label = (char*)malloc(10 * sizeof(char));
    sprintf(label, "lbl%d", label_counter++);
    return label;
}

char* generate_temp_var_name(){
    char* temp_name = malloc(32);
    do{
        sprintf(temp_name, "__internal_temp%d",temp_var_counter++);
    } while(symtab_lookup(temp_name) != NULL);
    return temp_name;
}

Node* create_temp_declaration(char* name, Node* initializer, char* type){
    Node* id = create_identifier_node(name);
    Node* decl = create_declaration_node(id, initializer);
    decl->already_linearized = 1;
    Node* typeNode = create_value_node(type);
    DeclaratorsListNode* decl_list = malloc(sizeof(DeclaratorsListNode));
    decl_list->base.type = DECLARATORS_LIST_NODE;
    decl_list->base.print = print_declarators_list_node;
    decl_list->type_specifier = typeNode;
    decl_list->declarators = malloc(sizeof(Node*));
    decl_list->declarators[0] = decl;
    decl_list->count = 1;
    decl_list->base.is_temp = 1;
    decl_list->base.already_linearized = 1;
    return (Node*)decl_list;
}

int is_simple_expression(Node* expr){
    if(!expr) return 1;
    switch (expr->type)
    {
    case CONSTANT_NODE:
    case IDENTIFIER_NODE:
    case STRING_LITERAL_NODE:
        return 1;
    default:
        return 0;
    }
}
typedef struct {
    Node** items;
    int count;
    int capacity;
} NodeList;

void append_node(NodeList* list, Node* node){
    if(list->count >= list->capacity){
        list->capacity = list->capacity * 2 + 1;
        list->items = realloc(list->items, list->capacity * sizeof(Node*));
    }
    list->items[list->count++] = node;
}
char* infer_type(Node* expr){
    if(!expr) return "__unknown";
    switch (expr->type){
        case IDENTIFIER_NODE: {
            IdentifierNode* id = (IdentifierNode*)expr;
            Symbol* sym = symtab_lookup(id->name);
            if(sym && sym->datatype) return sym->datatype;
            return "__unknown";
        }
        case CONSTANT_NODE:{
            ConstantNode* c = (ConstantNode*)expr;
            switch (c->const_type)
            {
            case CONST_INT: return "int";
            case CONST_FLOAT: return "float";
            case CONST_CHAR: return "char";
            default: return "__unknown";
            }
        }
        case STRING_LITERAL_NODE:{
            return "char*";
        }
        case TERNARY_OPERATOR_NODE:{
            TernaryOperatorNode* ternary = (TernaryOperatorNode*)expr;
            return infer_type(ternary->then_statement);
        }
        case POINTER_MEMBER_ACCESS_EXPRESSION_NODE: {
            PointerMemberAccessExpressionNode* point_expr = (PointerMemberAccessExpressionNode*)expr;
            char* ident = get_declarator_name(point_expr->field_name);
            Symbol* sym = symtab_lookup(ident);
            if(sym && sym->datatype) return sym->datatype;
            return "__unknown";
        }
        case MEMBER_ACCESS_EXPRESSION_NODE: {
            MemberAccessExpressionNode* member_expr = (MemberAccessExpressionNode*)expr;
            char* ident = get_declarator_name(member_expr->field_name);
            Symbol* sym = symtab_lookup(ident);
            if(sym && sym->datatype) return sym->datatype;
            return "__unknown";
        }
        case UNARY_OPERATOR_EXPRESSION_NODE: {
            UnaryOperatorExpressonNode* unary = (UnaryOperatorExpressonNode*)expr;
            return infer_type(unary->expression);
        }
        default:
            return "__unknown";
    }
}
Node* linearize_expression(Node* expr, NodeList* out){
    switch(expr->type){
        case CONSTANT_NODE:
        case STRING_LITERAL_NODE:
        case IDENTIFIER_NODE:{
            return expr;
        }
        case BINARY_OPERATION_NODE:{
            BinaryOperationNode* bin = (BinaryOperationNode*)expr;
            Node* left = linearize_expression(bin->left, out);
            Node* right = linearize_expression(bin->right, out);
            Node* new_bin = create_binary_operation_node(bin->op,left, right);
            char* temp_name = generate_temp_var_name();
            char* typeLeft = infer_type(left);
            char* typeRight = infer_type(right);
            char* type = (strcmp(typeLeft, "__unknown") != 0) ? typeLeft : 
                            (strcmp(typeRight,"__unknown") != 0) ? typeRight : "__unknown";
            symtab_add(temp_name, SYM_IDENTIFIER, type);
            Node* decl = create_temp_declaration(temp_name, new_bin,type);
            append_node(out, decl);
            
            return create_identifier_node(temp_name);
        }
        case PARENTHESIZED_EXPRESSION_NODE:{
            return linearize_expression(((ParenthesizedExpressionNode*)expr)->expression, out);
        }
        case FUNCTION_CALL_NODE: {
            FunctionCallNode* call = (FunctionCallNode*)expr;

            int all_args_simple = 1;
            if(call->arguments->type != EMPTY_STATEMENT_NODE){
                ArgumentsNode* args = (ArgumentsNode*)call->arguments;
                for(int i = 0; i < args->count; i++){
                    if(!is_simple_expression(args->arguments[i])){
                        all_args_simple = 0;
                        break;
                    }
                }
            }
            if(all_args_simple){
                return expr;
            }
            ArgumentsNode* args = (ArgumentsNode*)call->arguments;
            for(int i = 0; i < args->count; i++){
                args->arguments[i] = linearize_expression(args->arguments[i], out);
            }
            return expr;
        }
        case LOGICAL_OPERATION_NODE: {
            LogicalOperationNode* log = (LogicalOperationNode*)expr;
            Node* left = linearize_expression(log->left, out);
            Node* right = linearize_expression(log->right, out);
            Node* new_log = create_logical_operation_node(left, log->op, right);

            char* temp_name = generate_temp_var_name();
            symtab_add(temp_name, SYM_IDENTIFIER, "int");
            Node* decl = create_temp_declaration(temp_name, new_log, "int");
            append_node(out, decl);
            return create_identifier_node(temp_name);
        }
        case TERNARY_OPERATOR_NODE: {
            TernaryOperatorNode* ternary = (TernaryOperatorNode*)expr;
            Node* condition = linearize_expression(ternary->condition, out);
            Node* then_stm = linearize_expression(ternary->then_statement, out);
            Node* else_stm = linearize_expression(ternary->else_statement, out);
            Node* new_ternary = create_ternary_operator_node(condition,then_stm,else_stm);
            char* ternaryType = infer_type(new_ternary);
            char* type = (strcmp(ternaryType, "__unknown") != 0) ? ternaryType : "__unknown";
            char* temp_name = generate_temp_var_name();
            symtab_add(temp_name, SYM_IDENTIFIER, type);
            Node* decl = create_temp_declaration(temp_name, new_ternary, type);
            append_node(out, decl);
            return create_identifier_node(temp_name);
        }
        default:
            return expr;
    }
}

void generate_code(Node* node){

    switch (node->type) {
        case IF_NODE: {
            IfNode* if_node = (IfNode*)node;
            NodeList temps = {0};
            Node* simplified_condition = linearize_expression(if_node->condition, &temps);
            for(int i = 0; i < temps.count; i++){
                generate_code(temps.items[i]);
                fprintf(output_file,"\n");
            }
            char* else_label = generate_label();
            char* end_label = generate_label();
            fprintf(output_file, "if (");
            generate_code(simplified_condition);
            fprintf(output_file, ") goto %s;\n", else_label);
            if (if_node->else_statement) {
                generate_code(if_node->else_statement);
            }
            fprintf(output_file, "goto %s;\n", end_label);
            fprintf(output_file, "%s:\n", else_label);
            generate_code(if_node->then_statement);
            fprintf(output_file, "%s:", end_label);
            free(else_label);
            free(end_label);
            break;
        }
        case WHILE_NODE: {
            inLoop = 1;
            WhileNode* while_node = (WhileNode*)node;
            char* loop_label = generate_label();
            char* end_label = generate_label();
            current_loop_end_label = strdup(end_label);
            fprintf(output_file, "%s:\n", loop_label);
            NodeList temps = {0};
            Node* simplified = linearize_expression(while_node->condition, &temps);
            for(int i = 0; i < temps.count; i++){
                generate_code(temps.items[i]);
                fprintf(output_file,"\n");
            }
            char* temp_name = generate_temp_var_name();
            Node* temp_decl = create_temp_declaration(temp_name,create_unary_operator_expression_node("!",create_parenthesized_expression_node(simplified)), "int");
            generate_code(temp_decl);
            fprintf(output_file,"\n");
            fprintf(output_file, "if(%s) goto %s;\n",temp_name, end_label);
            generate_code(while_node->body);
            fprintf(output_file, "goto %s;\n", loop_label);
            fprintf(output_file, "%s:", end_label);
            inLoop = 0;
            break;
        }
        case DOWHILE_NODE: {
            inLoop = 1;
            char* label_start = generate_label();
            DoWhileNode* dw = (DoWhileNode*)node;
            fprintf(output_file, "%s:\n", label_start);
            generate_code(dw->do_statement);
            NodeList temps = {0};
            Node* simplified = linearize_expression(dw->condition, &temps);
            for(int i = 0; i < temps.count; i++){
                generate_code(temps.items[i]);
                fprintf(output_file,"\n");
            }
            
            fprintf(output_file, "if(");
            generate_code(simplified);
            fprintf(output_file,") goto %s;", label_start);
            free(label_start);
            inLoop = 0;
            break;
        }
        case FOR_NODE: {
            inLoop = 1;
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
                NodeList temps = {0};
                Node* simplified = linearize_expression(for_node->condition, &temps);
                for(int i = 0; i < temps.count; i++){
                    generate_code(temps.items[i]);
                    fprintf(output_file,"\n");
                }
                char* temp_name = generate_temp_var_name();
                Node* temp_decl = create_temp_declaration(temp_name, create_parenthesized_expression_node(simplified),"int");
                char* temp_not_name = generate_temp_var_name();
                generate_code(temp_decl);
                fprintf(output_file, ";\n");
                Node* temp_decl_not = create_temp_declaration(temp_not_name,create_unary_operator_expression_node("!",create_identifier_node(temp_name)), "int");
                generate_code(temp_decl_not);
                fprintf(output_file, ";\n");
                fprintf(output_file, "if (%s) goto %s;\n", temp_not_name, end_label);
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
            inLoop = 0;
            break;
        }
        case ALIGNAS_NODE: {
            AlignasNode* alignas = (AlignasNode*)node;
            fprintf(output_file, "alignas(");
            generate_code(alignas->expr);
            fprintf(output_file, ")");
            break;
        }
        case ALIGNOF_NODE: {
            AlignofNode* alignof = (AlignofNode*)node;
            fprintf(output_file, "alignof(");
            generate_code(alignof->type_name);
            fprintf(output_file, ")");
            break;
        }
        case EXPRESSIONS_LIST_NODE: {
            ExpressionsListNode* exprs_list = (ExpressionsListNode*)node;
            for(int i = 0; i < exprs_list->count; i++){
                generate_code(exprs_list->expressions[i]);
                if(exprs_list->count != 1 && i != exprs_list->count - 1){
                    fprintf(output_file, ", ");
                }
            }
            break;
        }
        case STATIC_ASSERT_NODE: {
            StaticAssertNode* static_assert = (StaticAssertNode*)node;
            fprintf(output_file, "static_assert(");
            generate_code(static_assert->expr);
            if(static_assert->message->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file, ", ");
                generate_code(static_assert->message);
            }
            fprintf(output_file, ");");
            break;
        }
        case PARENTHESIZED_EXPRESSION_NODE: {
            ParenthesizedExpressionNode* expr = (ParenthesizedExpressionNode*)node;
            fprintf(output_file,"(");
            generate_code(expr->expression);
            fprintf(output_file,")");
            break;
        }
        case COMPOUND_LITERALS_NODE: {
            CompoundLiteralsNode* compo = (CompoundLiteralsNode*)node;
            fprintf(output_file,"(");
            generate_code(compo->type_name);
            fprintf(output_file,")");
            generate_code(compo->initializer_list);
            break;
        }
        case GROUPED_DECLARATOR_NODE: {
            GroupedDeclaratorNode* group_decl = (GroupedDeclaratorNode*)node;
            fprintf(output_file,"(");
            generate_code(group_decl->declarator);
            fprintf(output_file,")");
            break;
        }
        case BINARY_OPERATION_NODE: {
            BinaryOperationNode* binop_node = (BinaryOperationNode*)node;
            if(binop_node->left->type == BINARY_OPERATION_NODE){
                fprintf(output_file,"(");
                generate_code(binop_node->left);
                fprintf(output_file,")");
            }else{
                generate_code(binop_node->left);
            }
            fprintf(output_file, " %s ",binop_node->op);
            if(binop_node->right->type == BINARY_OPERATION_NODE){
                fprintf(output_file,"(");
                generate_code(binop_node->right);
                fprintf(output_file,")");
            }else{
                generate_code(binop_node->right);
            }
            break;
        }
        case ENUM_NODE: {
            EnumNode* enum_node = (EnumNode*)node;
            fprintf(output_file, "enum ");
            generate_code(enum_node->identifier);
            if(enum_node->enumerators_list->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file, "{\n");
                generate_code(enum_node->enumerators_list);
                fprintf(output_file,"\n}");
            }
            break;
        }
        case ENUMERATORS_LIST_NODE: {
            EnumeratorsListNode* enum_list = (EnumeratorsListNode*)node;
            for(int i = 0; i < enum_list->count; i++){
                generate_code(enum_list->enumerators_list[i]);
                if(i != enum_list->count - 1){
                    fprintf(output_file, ",\n");
                }
            }
            break;
        }
        case ARRAY_EXPRESSION_NODE: {
            ArrayExpressionNode* array = (ArrayExpressionNode*)node;
            generate_code(array->identifier);
            fprintf(output_file,"[");
            generate_code(array->index);
            fprintf(output_file,"]");
            break;
        }
        case DESIGNATORS_LIST_NODE: {
            DesignatorsListNode* designators_list = (DesignatorsListNode*)node;
            for(int i = 0; i < designators_list->count; i++){
                generate_code(designators_list->designators[i]);
            }
            break;
        }
        case MEMBER_DESIGNATOR_NODE: {
            MemberDesignatorNode* member_designator = (MemberDesignatorNode*)node;
            fprintf(output_file, ".");
            generate_code(member_designator->identifier);
            break;
        }
        case ARRAY_DESIGNATOR_NODE: {
            ArrayDesignatorNode* array_designator = (ArrayDesignatorNode*)node;
            fprintf(output_file,"[");
            generate_code(array_designator->expression);
            fprintf(output_file,"]");
            break;
        }
        case STRUCTUNION_NODE: {
            StructUnionNode* su_node = (StructUnionNode*)node;
            generate_code(su_node->kind);
            fprintf(output_file," ");
            generate_code(su_node->identifier);
            if(su_node->body->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file, "{\n");
                generate_code(su_node->body);
                if(current_context == CONTEXT_DECLARATORS_LIST){
                    fprintf(output_file, "}");
                }else{
                    fprintf(output_file, "};");
                }
            }
            break;
        }
        case STRUCT_DECLARATIONS_LIST_NODE: {
            StructDeclarationsListNode* list = (StructDeclarationsListNode*)node;
            for(int i = 0; i < list->count; i++){
                Context temp = current_context;
                current_context = CONTEXT_DEFAULT;
                generate_code(list->declarations_list[i]);
                current_context = temp;
                fprintf(output_file, "\n");
            }
            break;
        }
        case STRUCT_DECLARATOR_NODE: {
            StructDeclaratorNode* declarator = (StructDeclaratorNode*)node;
            fprintf(output_file, " ");
            generate_code(declarator->declarator);
            if(declarator->bit_width->type != EMPTY_STATEMENT_NODE){
                fprintf(output_file, " : ");
                generate_code(declarator->bit_width);
            }
            break;
        }
        case LABELED_STATEMENT_NODE: {
            LabeledStatementNode* lbl_stmnt = (LabeledStatementNode*)node;
            generate_code(lbl_stmnt->identifier);
            fprintf(output_file, ": ");
            if(lbl_stmnt->statement->type == EXPRESSION_STATEMENT_NODE 
                && ((ExpressionStatementNode*)lbl_stmnt->statement)->expr->type == EMPTY_STATEMENT_NODE){
                fprintf(output_file," ;");
            } else if (lbl_stmnt->statement->type == COMPOUND_STATEMENT_NODE){
                fprintf(output_file,"{\n");
                generate_code(lbl_stmnt->statement);
                fprintf(output_file,"}");
            }else{
                fprintf(output_file, "\n");
                generate_code(lbl_stmnt->statement);
            }
            break;
        }
        case GOTO_NODE: {
            GotoNode* goto_node = (GotoNode*)node;
            fprintf(output_file, "goto ");
            generate_code(goto_node->identifier);
            fprintf(output_file, ";\n");
            break;
        }
        case INITIALIZERS_LIST_NODE: {

            InitializersListNode* list = (InitializersListNode*)node;
            fprintf(output_file, "{");
            for(int i = 0; i < list->count; i++){
                generate_code(list->initializers[i]);
                if(list->count != 1 && i != list->count - 1){
                    fprintf(output_file, ", ");
                }
            }
            fprintf(output_file, "}");
            break;
        }
        case INITIALIZER_NODE: {
            InitializerNode* initializer = (InitializerNode*)node;
            if(initializer->designation->type != EMPTY_STATEMENT_NODE){
                generate_code(initializer->designation);
                fprintf(output_file, " = ");
            }
            generate_code(initializer->initializer);
            break;
        }
        case CAST_EXPRESSION_NODE: {
            CastExpressionNode* cast_node = (CastExpressionNode*)node;
            fprintf(output_file, "(");
            generate_code(cast_node->type);
            fprintf(output_file, ")");
            generate_code(cast_node->expression);
            break;
        }
        case SIZEOF_NODE: {
            SizeofNode* sizeof_node = (SizeofNode*)node;
            fprintf(output_file, "sizeof(");
            generate_code(sizeof_node->expression);
            fprintf(output_file, ")");
            break;
        }
        case POINTER_MEMBER_ACCESS_EXPRESSION_NODE: {
            PointerMemberAccessExpressionNode* member_access = (PointerMemberAccessExpressionNode*)node;
            generate_code(member_access->pointer_expression);
            fprintf(output_file,"->");
            generate_code(member_access->field_name);
            break;
        }
        case MEMBER_ACCESS_EXPRESSION_NODE: {
            MemberAccessExpressionNode* member_access = (MemberAccessExpressionNode*)node;
            generate_code(member_access->object_expression);
            fprintf(output_file,".");
            generate_code(member_access->field_name);
            break;
        }
        case WRAPPER_NODE: {
            WrapperNode* wrapper = (WrapperNode*)node;
            generate_code(wrapper->wrapper);
            if(current_context != CONTEXT_POINTER){
                fprintf(output_file, " ");
            }
            generate_code(wrapper->inner_node);
            break;
        }
        case TERNARY_OPERATOR_NODE: {
            TernaryOperatorNode* ternary = (TernaryOperatorNode*)node;
            fprintf(output_file,"(");
            generate_code(ternary->condition);
            fprintf(output_file,") ? ");
            fprintf(output_file,"(");
            generate_code(ternary->then_statement);
            fprintf(output_file,") ");
            fprintf(output_file,": ");
            fprintf(output_file,"(");
            generate_code(ternary->else_statement);
            fprintf(output_file,")");
            break;
        }
        case RETURN_NODE: {
            ReturnNode* return_node = (ReturnNode*)node;
            if(return_node->expression->type != EMPTY_STATEMENT_NODE){
                NodeList temps = {0};
                Node* simplified = linearize_expression(return_node->expression, &temps);
                for(int i =0; i < temps.count; i++){
                    generate_code(temps.items[i]);
                    fprintf(output_file, "\n");
                }
                return_node->expression = simplified;
            }
            
            fprintf(output_file,"return ");
            if(return_node->expression->type != EMPTY_STATEMENT_NODE){
                generate_code(return_node->expression);
            }
            fprintf(output_file, ";");
            break;
        }
        case CONTINUE_NODE: {
            fprintf(output_file, "continue;");
            break;
        }
        case CASE_NODE: {
            CaseNode* case_node = (CaseNode*)node;
            fprintf(output_file,"case ");
            generate_code(case_node->expression);
            fprintf(output_file, ":{\n");
            generate_code(case_node->body);
            fprintf(output_file,"}");
            break;
        }
        case SWITCH_NODE: {
            SwitchNode* sw_node = (SwitchNode*)node;
            fprintf(output_file, "switch(");
            generate_code(sw_node->expression);
            fprintf(output_file, "){\n");
            generate_code(sw_node->body);
            fprintf(output_file,"}");
            break;
        }
        case BREAK_NODE: {
            if(inLoop){
                printf("DSADSAD\n");
                fprintf(output_file,"goto %s;",current_loop_end_label);
                break;
            }
            fprintf(output_file, "break;");
            break;
        }
        case DEFAULT_NODE: {
            DefaultNode* default_node = (DefaultNode*)node;
            fprintf(output_file,"default: {\n");
            generate_code(default_node->body);
            fprintf(output_file,"}");
            break;
        }
        case ARRAY_DECLARATION_NODE: {
            ArrayDeclarationNode* array_node = (ArrayDeclarationNode*)node;
            generate_code(array_node->declarator);
            fprintf(output_file,"[");
            Context temp = current_context;
            current_context = CONTEXT_ARRAY_INDEX;
            generate_code(array_node->index_expression);
            current_context = temp;
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
            NodeList temps = {0};
            if(func_call_node->arguments->type != EMPTY_STATEMENT_NODE){
                ArgumentsNode* args = (ArgumentsNode*)func_call_node->arguments;
                for(int i = 0 ; i < args->count; i++){
                    args->arguments[i] = linearize_expression(args->arguments[i], &temps);
                }
                for(int i = 0; i < temps.count; i++){
                    generate_code(temps.items[i]);
                    fprintf(output_file,"\n");
                }
            }
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
            NodeList temps = {0};
            Node* simplified_right = linearize_expression(assignment_node->right, &temps);
            for(int i = 0; i < temps.count; i++){
                generate_code(temps.items[i]);
                fprintf(output_file,"\n");
            }
            generate_code(assignment_node->left);
            fprintf(output_file," %s ",assignment_node->op);
            generate_code(simplified_right);
            fprintf(output_file,";");
            break;
        }
        case PREFIX_DECREMENT_NODE: {
            PrefixDecrement* decr = (PrefixDecrement*)node;
            fprintf(output_file, "--");
            generate_code(decr->expression);
            break;
        }
        case POSTFIX_DECREMENT_NODE: {
            PostfixDecrement* decr = (PostfixDecrement*)node;
            generate_code(decr->expression);
            fprintf(output_file, "--");
            break;
        }
        case PREFIX_INCREMENT_NODE: {
            PrefixIncrement* incr = (PrefixIncrement*)node;
            fprintf(output_file, "++");
            generate_code(incr->expression);
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
                fprintf(output_file,";");
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
            if(decl_list->base.is_temp){
                Context temp = current_context;
                current_context = CONTEXT_DECLARATORS_LIST;
                generate_code(decl_list->type_specifier);
                current_context = temp;
                for(int i = 0; i < decl_list->count; i++){
                    DeclarationNode* decl_node = (DeclarationNode*)decl_list->declarators[i];
                    generate_code(decl_list->declarators[i]);
                    if(i != decl_list->count - 1 && decl_list->count != 1){
                        fprintf(output_file, ",");
                    }
                }
            }else{
                
                NodeList temps = {0};
                for(int i = 0; i < decl_list->count; i++){
                    DeclarationNode* decl_node = (DeclarationNode*)decl_list->declarators[i];
                    if(!decl_node->base.already_linearized && decl_node->initializer->type != EMPTY_STATEMENT_NODE){
                        Node* simplified = linearize_expression(decl_node->initializer, &temps);
                        decl_node->initializer = simplified;
                        decl_node->base.already_linearized = 1;
                        
                    }
                }
                for(int j = 0; j < temps.count; j++){
                    generate_code(temps.items[j]);
                    fprintf(output_file,"\n");
                }
                Context temp = current_context;
                current_context = CONTEXT_DECLARATORS_LIST;
                generate_code(decl_list->type_specifier);
                current_context = temp;
                for(int i = 0; i < decl_list->count; i++){
                    generate_code(decl_list->declarators[i]);
                    if(i != decl_list->count - 1 && decl_list->count != 1){
                        fprintf(output_file, ",");
                    }
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
            Context temp = current_context;
            current_context = CONTEXT_POINTER;
            PointerNode* pointer = (PointerNode*)node;
            generate_code(pointer->point);
            generate_code(pointer->declarator);
            current_context = temp;
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
            } else if(constant->const_type == CONST_FLOAT){
                fprintf(output_file,"%f",constant->value.f_value);
            } else if(constant->const_type == CONST_CHAR){
                fprintf(output_file,"'%c'",constant->value.c_value);
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
