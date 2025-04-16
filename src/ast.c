#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/ast.h"

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

Node* create_constant_char_node(char value){
    ConstantNode* node = (ConstantNode*)malloc(sizeof(ConstantNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for ConstantNode\n");
        exit(1);
    }
    node->base.type = CONSTANT_NODE;
    node->const_type = CONST_CHAR;
    node->value.c_value = value;
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
    return (Node*)node;
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
        fprintf(stderr, "Error: Failed to allocate memory for Node\n");
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

Node* create_function_declaration_node(Node* return_type, Node* declarator, Node* body){
    FunctionDeclarationNode* node = (FunctionDeclarationNode*)malloc(sizeof(FunctionDeclarationNode));
    node->base.type = FUNCTION_DECLARATION_NODE;
    node->base.print = print_function_declaration_node;
    node->declarator = declarator;
    node->return_type = return_type;
    node->body = body;
    return (Node*)node;
}

Node* create_string_literal_node(char* value){
    StringNode* node = (StringNode*)malloc(sizeof(StringNode));
    node->base.type = STRING_LITERAL_NODE;
    node->base.print = print_string_literal_node;
    node->value = strdup(value);
    return (Node*)node;
}

Node* create_while_node(Node* condition, Node* body){
    WhileNode* node = (WhileNode*)malloc(sizeof(WhileNode));
    node->base.type = WHILE_NODE;
    node->base.print = print_while_node;
    node->condition = condition;
    node->body = body;
    return (Node*)node;
}

Node* create_do_while_node(Node* do_statement, Node* condition){
    DoWhileNode* node = (DoWhileNode*)malloc(sizeof(WhileNode));
    node->base.type = DOWHILE_NODE;
    node->base.print = print_do_while_node;
    node->do_statement = do_statement;
    node->condition = condition;
    return (Node*)node;
}

Node* create_switch_node(Node* expression, Node* body){
    SwitchNode* node = (SwitchNode*)malloc(sizeof(SwitchNode));
    node->base.type = SWITCH_NODE;
    node->base.print = print_switch_node;
    node->expression = expression;
    node->body = body;
    return (Node*)node;
}

Node* create_case_node(Node* expression, Node* body){
    CaseNode* node = (CaseNode*)malloc(sizeof(CaseNode));
    node->base.type = CASE_NODE;
    node->base.print = print_case_node;
    node->expression = expression;
    node->body = body;
    return (Node*)node;
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
    return (Node*)node;
}

Node* create_return_node(Node* expression){
    ReturnNode* node = (ReturnNode*)malloc(sizeof(ReturnNode));
    node->base.type = RETURN_NODE;
    node->base.print = print_return_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_goto_node(Node* identifier){
    GotoNode* node = (GotoNode*)malloc(sizeof(GotoNode));
    node->base.type = GOTO_NODE;
    node->base.print = print_goto_node;
    node->identifier = identifier;
    return (Node*)node;
}

Node* create_continue_node(){
    ContinueNode* node = (ContinueNode*)malloc(sizeof(ContinueNode));
    node->base.type = CONTINUE_NODE;
    node->base.print = print_continue_node;
    return (Node*)node;
}

Node* create_prefix_increment_node(Node* expression){
    PrefixIncrement* node = (PrefixIncrement*)malloc(sizeof(PrefixIncrement));
    node->base.type = PREFIX_INCREMENT_NODE;
    node->base.print = print_prefix_increment_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_postfix_increment_node(Node* expression){
    PostfixIncrement* node = (PostfixIncrement*)malloc(sizeof(PostfixIncrement));
    node->base.type = POSTFIX_INCEMENT_NODE;
    node->base.print = print_postfix_increment_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_prefix_decrement_node(Node* expression){
    PrefixDecrement* node = (PrefixDecrement*)malloc(sizeof(PrefixDecrement));
    node->base.type = PREFIX_DECREMENT_NODE;
    node->base.print = print_prefix_decrement_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_postfix_decrement_node(Node* expression){
    PostfixDecrement* node = (PostfixDecrement*)malloc(sizeof(PostfixDecrement));
    node->base.type = POSTFIX_DECREMENT_NODE;
    node->base.print = print_postfix_decrement_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_unary_operator_expression_node(char* unary_operator, Node* expression){
    UnaryOperatorExpressonNode* node = (UnaryOperatorExpressonNode*)malloc(sizeof(UnaryOperatorExpressonNode));
    node->base.type = UNARY_OPERATOR_EXPRESSION_NODE;
    node->base.print = print_unary_operator_expression_node;
    node->unary_operator = strdup(unary_operator);
    node->expression = expression;
    return (Node*)node;
}

Node* create_pointer_node(Node* point, Node* declarator){
    PointerNode* node = (PointerNode*)malloc(sizeof(PointerNode));
    node->base.type = POINTER_NODE;
    node->base.print = print_pointer_node;
    node->declarator = declarator;
    node->point = point;
    return (Node*)node;
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
    return (Node*)node;
}

Node* create_arguments_node(Node** arguments, int count){
    ArgumentsNode* node = (ArgumentsNode*)malloc(sizeof(ArgumentsNode));
    node->arguments = arguments;
    node->count = count;
    node->base.type = ARGUMENTS_NODE;
    node->base.print= print_arguments_node;
    return (Node*)node;
}

Node* create_function_call_node(Node* name, Node* arguments){
    FunctionCallNode* node = (FunctionCallNode*)malloc(sizeof(FunctionCallNode));
    node->name = name;
    node->arguments = arguments;
    node->base.type = FUNCTION_CALL_NODE;
    node->base.print = print_function_call_node;
    return (Node*)node;
}

Node* create_array_declaration_node(Node* declarator, Node* index_expression){
    ArrayDeclarationNode* node = (ArrayDeclarationNode*)malloc(sizeof(ArrayDeclarationNode));
    node->declarator = declarator;
    node->index_expression = index_expression;
    node->base.type = ARRAY_DECLARATION_NODE;
    node->base.print = print_array_declaration_node;
    return (Node*)node;
}

Node* create_array_expression_node(Node* identifier, Node* index){
    ArrayExpressionNode* node = (ArrayExpressionNode*)malloc(sizeof(ArrayExpressionNode));
    node->identifier = identifier;
    node->index = index;
    node->base.type = ARRAY_EXPRESSION_NODE;
    node->base.print = print_array_expression_node;
    return (Node*)node;
}

Node* create_sizeof_node(Node* expression){
    SizeofNode* node = (SizeofNode*)malloc(sizeof(SizeofNode)); 
    node->expression = expression;
    node->base.type = SIZEOF_NODE;
    node->base.print = print_sizeof_node;
    return (Node*)node;
}

Node* create_value_node(char* value){
    ValueNode* node = (ValueNode*)malloc(sizeof(ValueNode));
    node->value = strdup(value);
    node->base.print = print_value_node;
    node->base.type = VALUE_NODE;
    return (Node*)node;
}

Node* create_member_access_expression_node(Node* object_expression, Node* field_name){
    MemberAccessExpressionNode* node = (MemberAccessExpressionNode*)malloc(sizeof(MemberAccessExpressionNode));
    node->base.type = MEMBER_ACCESS_EXPRESSION_NODE;
    node->base.print = print_member_access_expression_node;
    node->object_expression = object_expression;
    node->field_name = field_name;
    return (Node*)node;
}

Node* create_pointer__member_access_expression_node(Node* pointer_expression, Node* field_name){
    PointerMemberAccessExpressionNode* node = (PointerMemberAccessExpressionNode*)malloc(sizeof(PointerMemberAccessExpressionNode));
    node->base.type = POINTER_MEMBER_ACCESS_EXPRESSION_NODE;
    node->base.print = print_pointer_member_access_expression_node;
    node->pointer_expression = pointer_expression;
    node->field_name = field_name;
    return (Node*)node;
}

Node* create_structunion_node(Node* kind, Node* identifier, Node* body){
    StructUnionNode* node = (StructUnionNode*)malloc(sizeof(StructUnionNode));
    node->kind = kind;
    node->identifier = identifier;
    node->body = body;
    node->base.print = print_structunion_node;
    node->base.type = STRUCTUNION_NODE;
    return (Node*)node;
}

Node* create_struct_declarator_node(Node* declarator, Node* bit_width){
    StructDeclaratorNode* node = (StructDeclaratorNode*)malloc(sizeof(StructDeclaratorNode));
    node->declarator = declarator;
    node->bit_width = bit_width;
    node->base.print = print_struct_declarator_node;
    node->base.type = STRUCT_DECLARATOR_NODE;
    return (Node*)node;
}

Node* create_struct_declarations_list_node(Node** declarations, int count){
    StructDeclarationsListNode* node = (StructDeclarationsListNode*)malloc(sizeof(StructDeclarationsListNode));
    node->declarations_list = declarations;
    node->count = count;
    node->base.type = STRUCT_DECLARATIONS_LIST_NODE;
    node->base.print= print_struct_declarations_list_node;
    return (Node*)node;
}

Node* create_cast_expression_node(Node* type, Node* expression){
    CastExpressionNode* node = (CastExpressionNode*)malloc(sizeof(CastExpressionNode));
    node->type = type;
    node->expression = expression;
    node->base.type = CAST_EXPRESSION_NODE;
    node->base.print = print_cast_expression_node;
    return (Node*)node;
}

Node* create_labeled_statement_node(Node* identifier, Node* statement){
    LabeledStatementNode* node= (LabeledStatementNode*)malloc(sizeof(LabeledStatementNode));
    node->identifier = identifier;
    node->statement = statement;
    node->base.type = LABELED_STATEMENT_NODE;
    node->base.print = print_labeled_statement_node;
    return (Node*)node; 
}

Node* create_wrapper_node(Node* wrapper, Node* inner_node){
    WrapperNode* node = (WrapperNode*)malloc(sizeof(WrapperNode));
    node->base.type = WRAPPER_NODE;
    node->base.print = print_wrapper_node;
    node->wrapper = wrapper;
    node->inner_node = inner_node;
    return (Node*)node;
}

Node* create_enum_node(Node* identifier, Node* enumerators_list){
    EnumNode* node = (EnumNode*)malloc(sizeof(EnumNode));
    node->base.type = ENUM_NODE;
    node->base.print = print_enum_node;
    node->identifier = identifier;
    node->enumerators_list = enumerators_list;
    return (Node*)node;
}

Node* create_enumerators_list_node(Node** enumerators_list, int count){
    EnumeratorsListNode* node = (EnumeratorsListNode*)malloc(sizeof(EnumeratorsListNode));
    node->base.type = ENUMERATORS_LIST_NODE;
    node->base.print = print_enumerators_list_node;
    node->enumerators_list = enumerators_list;
    node->count = count;
    return (Node*)node;
}

Node* create_member_designator_node(Node* identifier){
    MemberDesignatorNode* node = (MemberDesignatorNode*)malloc(sizeof(MemberDesignatorNode));
    node->base.type = MEMBER_DESIGNATOR_NODE;
    node->base.print = print_member_designator_node;
    node->identifier = identifier;
    return (Node*)node;
}

Node* create_array_designator_node(Node* expression){
    ArrayDesignatorNode* node = (ArrayDesignatorNode*)malloc(sizeof(ArrayDesignatorNode));
    node->base.type = ARRAY_DESIGNATOR_NODE;
    node->base.print = print_array_designator_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_designator_list_node(Node** designators, int count){
    DesignatorsListNode* node = (DesignatorsListNode*)malloc(sizeof(DesignatorsListNode));
    node->base.type = DESIGNATORS_LIST_NODE;
    node->base.print = print_designators_list_node;
    node->designators = designators;
    node->count = count;
    return (Node*)node;
}

Node* create_initializer_node(Node* initializer, Node* desgnation){
    InitializerNode* node = (InitializerNode*)malloc(sizeof(InitializerNode));
    node->base.type = INITIALIZER_NODE;
    node->base.print = print_initializer_node;
    node->initializer = initializer;
    node->designation = desgnation;
    return (Node*)node;
}

Node* create_initializers_list_node(Node** initializers, int count){
    InitializersListNode* node = (InitializersListNode*)malloc(sizeof(InitializersListNode));
    node->base.type = INITIALIZERS_LIST_NODE;
    node->base.print = print_initializers_list_node;
    node->initializers = initializers;
    node->count = count;
    return (Node*)node;
}

Node* create_declarators_list_node(Node** declarators, int count){
    DeclaratorsListNode* node = (DeclaratorsListNode*)malloc(sizeof(DeclaratorsListNode));
    node->base.type = DECLARATORS_LIST_NODE;
    node->base.print = print_declarators_list_node;
    node->declarators = declarators;
    node->count = count;
    return (Node*)node;
}

Node* create_grouped_declarator_node(Node* declarator){
    GroupedDeclaratorNode* node = (GroupedDeclaratorNode*)malloc(sizeof(GroupedDeclaratorNode));
    node->base.type = GROUPED_DECLARATOR_NODE;
    node->base.print = print_grouped_declarator_node;
    node->declarator = declarator;
    return (Node*)node;
}

Node* create_compound_literals_node(Node* type_name, Node* initializer_list){
    CompoundLiteralsNode* node = (CompoundLiteralsNode*)malloc(sizeof(CompoundLiteralsNode));
    node->base.type = COMPOUND_LITERALS_NODE;
    node->base.print = print_compound_literals_node;
    node->initializer_list = initializer_list;
    node->type_name = type_name;
    return (Node*)node;
}

Node* create_parenthesized_expression_node(Node* expression){
    ParenthesizedExpressionNode* node = (ParenthesizedExpressionNode*)malloc(sizeof(ParenthesizedExpressionNode));
    node->base.type = PARENTHESIZED_EXPRESSION_NODE;
    node->base.print = print_parenthesized_expression_node;
    node->expression = expression;
    return (Node*)node;
}

Node* create_static_assert_node(Node* expr, Node* message){
    StaticAssertNode* node = (StaticAssertNode*)malloc(sizeof(StaticAssertNode));
    node->base.type = STATIC_ASSERT_NODE;
    node->base.print = print_static_assert_node;
    node->expr = expr;
    node->message = message;
    return (Node*)node;
}

Node* create_expressions_list_node(Node** expressions, int count){
    ExpressionsListNode* node = (ExpressionsListNode*)malloc(sizeof(ExpressionsListNode));
    node->base.type = EXPRESSIONS_LIST_NODE;
    node->base.print = print_expressions_list_node;
    node->expressions = expressions;
    node->count = count;
    return (Node*)node;
}

Node* create_alignof_node(Node* type_name){
    AlignofNode* node = (AlignofNode*)malloc(sizeof(AlignofNode));
    node->base.type = ALIGNOF_NODE;
    node->base.print = print_alignof_node;
    node->type_name = type_name;
    return (Node*)node;
}

Node* create_alignas_node(Node* expr){
    AlignasNode* node = (AlignasNode*)malloc(sizeof(AlignasNode));
    node->base.type = ALIGNAS_NODE;
    node->base.print = print_alignas_node;
    node->expr = expr;
    return (Node*)node;
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
    } else if (const_node->const_type == CONST_CHAR){
        printf(COLOR_BLUE "Constant (char) Node: " COLOR_GREEN " %c\n" COLOR_RESET, const_node->value.c_value);        
    }else {
        printf(COLOR_RED "Constant: Unknown type" COLOR_RESET "\n");
    }
}

void print_binary_operation_node(Node* node){
    BinaryOperationNode* bin_node = (BinaryOperationNode*)node;
    print_indent();
    printf(COLOR_BLUE "Binary operation Node: " COLOR_YELLOW "%s" COLOR_RESET "\n", bin_node->op);
    indent_level++;
    if(bin_node->left != NULL){
    print_indent();
        printf("Left:\n");
        indent_level++;
        bin_node->left->print(bin_node->left);
        indent_level--;
    }

    if(bin_node->right != NULL){
        print_indent();
        printf("Right:\n");
        indent_level++;
        bin_node->right->print(bin_node->right);
        indent_level--;
    }
    indent_level--;
}

void print_declaration_node(Node* node){
    DeclarationNode* declaration_node = (DeclarationNode*)node;
    print_indent();
    printf( COLOR_BLUE "Declaration Node: " COLOR_RESET "\n");
    indent_level++;
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
    if(if_node->else_statement->type != EMPTY_STATEMENT_NODE){
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
    printf("Function type: \n");
    indent_level++;
    func_decl_node->return_type->print(func_decl_node->return_type);
    indent_level--;
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
    if(ternary_node->else_statement->type != EMPTY_STATEMENT_NODE){
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
    if(return_node->expression->type != EMPTY_STATEMENT_NODE){
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
    PrefixIncrement* incr_node = (PrefixIncrement*)node;
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
    printf("Point expression: \n");
    indent_level++;
    pointer_node->point->print(pointer_node->point);
    indent_level--;
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
    print_indent();
    printf("Declarator: \n");
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

void print_arguments_node(Node* node){
    ParametersNode* arguments_node = (ParametersNode*)node;
    print_indent();
    printf(COLOR_BLUE "Arguments Node, count: %d " COLOR_RESET "\n", arguments_node->count);
    indent_level++;
    for(int i = 0; i < arguments_node->count; i++){
        print_indent();
        printf("Argument [%d]: \n",i);
        indent_level++;
        arguments_node->parameters[i]->print(arguments_node->parameters[i]);
        indent_level--;
    }
}

void print_function_call_node(Node* node){
    FunctionCallNode* function_call_node = (FunctionCallNode*)node;
    print_indent();
    printf(COLOR_BLUE "Function call Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("function name: \n");
    indent_level++;
    function_call_node->name->print(function_call_node->name);
    indent_level--;
    print_indent();
    printf("arguments: \n");
    indent_level++;
    function_call_node->arguments->print(function_call_node->arguments);
    indent_level--;
    indent_level--;
}

void print_array_declaration_node(Node* node){
    ArrayDeclarationNode* array_node = (ArrayDeclarationNode*)node;
    print_indent();
    printf(COLOR_BLUE "Array declaration Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Array declarator: \n");
    indent_level++;
    array_node->declarator->print(array_node->declarator);
    indent_level--;
    print_indent();
    printf("Array index: \n");
    indent_level++;
    array_node->index_expression->print(array_node->index_expression);
    indent_level--;
    indent_level--;
}

void print_array_expression_node(Node* node){
    ArrayExpressionNode* array_node = (ArrayExpressionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Array expression Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Array identifier: \n");
    indent_level++;
    array_node->identifier->print(array_node->identifier);
    indent_level--;
    print_indent();
    printf("Array index: \n");
    indent_level++;
    array_node->index->print(array_node->index);
    indent_level--;
    indent_level--;
}

void print_sizeof_node(Node* node){
    SizeofNode* sizeof_node = (SizeofNode*)node;
    print_indent();
    printf(COLOR_BLUE "Sizeof Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Expression: \n");
    indent_level++;
    sizeof_node->expression->print(sizeof_node->expression);
    indent_level--;
    indent_level--;
}

void print_value_node(Node* node){
    ValueNode* val_node = (ValueNode*)node;
    print_indent();
    printf(COLOR_BLUE "Value Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Value: " COLOR_GREEN "%s" COLOR_RESET "\n", val_node->value);
    indent_level--;
}

void print_member_access_expression_node(Node* node){
    MemberAccessExpressionNode* mem_node = (MemberAccessExpressionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Member access expression Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Object expression: \n");
    indent_level++;
    mem_node->object_expression->print(mem_node->object_expression);
    indent_level--;
    print_indent();
    printf("Field name: \n");
    indent_level++;
    mem_node->field_name->print(mem_node->field_name);
    indent_level--;
    indent_level--;
}

void print_pointer_member_access_expression_node(Node* node){
    PointerMemberAccessExpressionNode* mem_node = (PointerMemberAccessExpressionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Pointer member access expression Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Pointer expression: \n");
    indent_level++;
    mem_node->pointer_expression->print(mem_node->pointer_expression);
    indent_level--;
    print_indent();
    printf("Field name: \n");
    indent_level++;
    mem_node->field_name->print(mem_node->field_name);
    indent_level--;
    indent_level--;
}

void print_structunion_node(Node* node){
    StructUnionNode* su_node = (StructUnionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Struct Union Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Kind: \n");
    indent_level++;
    su_node->kind->print(su_node->kind);
    indent_level--;
    print_indent();
    printf("Identifier: \n");
    indent_level++;
    su_node->identifier->print(su_node->identifier);
    indent_level--;
    print_indent();
    printf("Body: \n");
    indent_level++;
    su_node->body->print(su_node->body);
    indent_level--;
    indent_level--;
}

void print_struct_declarator_node(Node* node){
    StructDeclaratorNode* dec_node = (StructDeclaratorNode*)node;
    print_indent();
    printf(COLOR_BLUE "Struct declarator Node" COLOR_RESET "\n");
    indent_level++;
    if(dec_node->declarator->type != EMPTY_STATEMENT_NODE){
        print_indent();
        printf("Declarator: \n");
        indent_level++;
        dec_node->declarator->print(dec_node->declarator);
        indent_level--;
    }

    if(dec_node->bit_width->type != EMPTY_STATEMENT_NODE){
        print_indent();
        printf("Bit width: \n");
        indent_level++;
        dec_node->bit_width->print(dec_node->bit_width);
        indent_level--;
    }
    indent_level--;
}

void print_struct_declarations_list_node(Node* node){
    StructDeclarationsListNode* decs_node = (StructDeclarationsListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Struct declarations Node, count: %d" COLOR_RESET "\n",decs_node->count);
    indent_level++;
    for(int i = 0; i < decs_node->count; i++){
        print_indent();
        printf("declaration[%d]: \n",i);
        indent_level++;
        decs_node->declarations_list[i]->print(decs_node->declarations_list[i]);
        indent_level--;
    }
    indent_level--;
}

void print_cast_expression_node(Node* node){
    CastExpressionNode* cast_node = (CastExpressionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Cast expression Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Type: \n");
    indent_level++;
    cast_node->type->print(cast_node->type);
    indent_level--;
    print_indent();
    printf("Expression: \n");
    indent_level++;
    cast_node->expression->print(cast_node->expression);
    indent_level--;
    indent_level--;
}

void print_labeled_statement_node(Node* node){
    LabeledStatementNode* lbl_node = (LabeledStatementNode*)node;
    print_indent();
    printf(COLOR_BLUE "Labeled statement Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Label identifier: \n");
    indent_level++;
    lbl_node->identifier->print(lbl_node->identifier);
    indent_level--;
    print_indent();
    printf("Label statement: \n");
    indent_level++;
    lbl_node->statement->print(lbl_node->statement);
    indent_level--;
    indent_level--;
}

void print_wrapper_node(Node* node){
    WrapperNode* wrap_node = (WrapperNode*)node;
    print_indent();
    printf(COLOR_BLUE "Wrapper Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Wrapper: \n");
    indent_level++;
    wrap_node->wrapper->print(wrap_node->wrapper);
    indent_level--;
    print_indent();
    printf("Inner expression: \n");
    indent_level++;
    wrap_node->inner_node->print(wrap_node->inner_node);
    indent_level--;
    indent_level--;
}

void print_enum_node(Node* node){
    EnumNode* enum_node = (EnumNode*)node;
    print_indent();
    printf(COLOR_BLUE "Enum Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Enum identifier: \n");
    indent_level++;
    enum_node->identifier->print(enum_node->identifier);
    indent_level--;
    print_indent();
    printf("Enumerators list: \n");
    indent_level++;
    enum_node->enumerators_list->print(enum_node->enumerators_list);
    indent_level--;
    indent_level--;
}

void print_enumerators_list_node(Node* node){
    EnumeratorsListNode* list_node = (EnumeratorsListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Enumerators list Node, count: %d" COLOR_RESET "\n",list_node->count);
    indent_level++;
    for(int i = 0; i < list_node->count; i++){
        print_indent();
        printf("enumerators[%d]: \n",i);
        indent_level++;
        list_node->enumerators_list[i]->print(list_node->enumerators_list[i]);
        indent_level--;
    }
    indent_level--;
}

void print_member_designator_node(Node* node){
    MemberDesignatorNode* mem_des = (MemberDesignatorNode*)node;
    print_indent();
    printf(COLOR_BLUE "Member designator Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Identifier: \n");
    indent_level++;
    mem_des->identifier->print(mem_des->identifier);
    indent_level--;
    indent_level--;
}

void print_array_designator_node(Node* node){
    ArrayDesignatorNode* arr_des = (ArrayDesignatorNode*)node;
    print_indent();
    printf(COLOR_BLUE "Array designator Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Expression: \n");
    indent_level++;
    arr_des->expression->print(arr_des->expression);
    indent_level--;
    indent_level--;
}

void print_designators_list_node(Node* node){
    DesignatorsListNode* design_list = (DesignatorsListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Designators list Node, count: %d" COLOR_RESET "\n",design_list->count);
    indent_level++;
    for(int i = 0; i < design_list->count; i++){
        print_indent();
        printf("designator[%d]: \n",i);
        indent_level++;
        design_list->designators[i]->print(design_list->designators[i]);
        indent_level--;
    }
    indent_level--;
}

void print_initializer_node(Node* node){
    InitializerNode* initializer_node = (InitializerNode*)node;
    print_indent();
    printf(COLOR_BLUE "Initializer Node: " COLOR_RESET "\n");
    if(initializer_node->designation->type != EMPTY_STATEMENT_NODE){
        indent_level++;
        print_indent();
        printf("Designation: \n");
        indent_level++;
        initializer_node->designation->print(initializer_node->designation);
        indent_level--;
        indent_level--;
    }
    indent_level++;
    print_indent();
    printf("Initializer: \n");
    indent_level++;
    initializer_node->initializer->print(initializer_node->initializer);
    indent_level--;
    indent_level--;
}

void print_initializers_list_node(Node* node){
    InitializersListNode* initalizers_list = (InitializersListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Initializers list Node: " COLOR_RESET "\n");
    indent_level++;
    for(int i = 0; i < initalizers_list->count; i++){
        print_indent();
        printf("initializer[%d]: \n",i);
        indent_level++;
        initalizers_list->initializers[i]->print(initalizers_list->initializers[i]);
        indent_level--;
    }
    indent_level--;
}

void print_declarators_list_node(Node* node){
    DeclaratorsListNode* declarators_list = (DeclaratorsListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Declarator list Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Specifier: \n");
    indent_level++;
    declarators_list->type_specifier->print(declarators_list->type_specifier);
    indent_level--;
    print_indent();
    printf("Declarators: \n");
    indent_level++;
    for(int i = 0; i < declarators_list->count; i++){
        print_indent();
        printf("declarator[%d]: \n",i);
        indent_level++;
        declarators_list->declarators[i]->print(declarators_list->declarators[i]);
        indent_level--;
    }
    indent_level--;
    indent_level--;
}

void print_grouped_declarator_node(Node* node){
    print_indent();
    printf(COLOR_BLUE "Grouped declarator Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Declarator: \n");
    GroupedDeclaratorNode* group = (GroupedDeclaratorNode*)node;
    indent_level++;
    group->declarator->print(group->declarator);
    indent_level--;
    indent_level--;
}

void print_compound_literals_node(Node* node){
    print_indent();
    printf(COLOR_BLUE "Compound literals Node: " COLOR_RESET "\n");
    indent_level++;
    CompoundLiteralsNode* literals =  (CompoundLiteralsNode*)node;
    print_indent();
    printf("Initializer list: \n");
    indent_level++;
    literals->initializer_list->print(literals->initializer_list);
    indent_level--;
    print_indent();
    printf("Type: \n");
    indent_level++;
    literals->type_name->print(literals->type_name);
    indent_level--;
    indent_level--;
}

void print_parenthesized_expression_node(Node* node){
    ParenthesizedExpressionNode* expr = (ParenthesizedExpressionNode*)node;
    print_indent();
    printf(COLOR_BLUE "Parenthesized expression Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Expression: \n");
    indent_level++;
    expr->expression->print(expr->expression);
    indent_level--;
    indent_level--;
}

void print_static_assert_node(Node* node){
    StaticAssertNode* static_assert_node = (StaticAssertNode*)node;
    print_indent();
    printf(COLOR_BLUE "Static assert Node:" COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Expr: \n");
    indent_level++;
    static_assert_node->expr->print(static_assert_node->expr);
    indent_level--;
    if(static_assert_node->message->type != EMPTY_STATEMENT_NODE){
        print_indent();
        printf("Message: \n");
        indent_level++;
        static_assert_node->message->print(static_assert_node->message);
        indent_level--;
    }
    indent_level--;
}

void print_expressions_list_node(Node* node){
    ExpressionsListNode* expressions_list = (ExpressionsListNode*)node;
    print_indent();
    printf(COLOR_BLUE "Expressions list Node:" COLOR_RESET "\n");
    indent_level++;
    for(int i = 0; i < expressions_list->count; i++){
        print_indent();
        printf("expression[%d]: \n",i);
        indent_level++;
        expressions_list->expressions[i]->print(expressions_list->expressions[i]);
        indent_level--;
    }
    indent_level--;
}

void print_alignof_node(Node* node){
    AlignofNode* alignof_node = (AlignofNode*)node;
    print_indent();
    printf(COLOR_BLUE "Alignof Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Type name: \n");
    indent_level++;
    alignof_node->type_name->print(alignof_node->type_name);
    indent_level--;
    indent_level--;
}

void print_alignas_node(Node* node){
    AlignasNode* alignas_node = (AlignasNode*)node;
    print_indent();
    printf(COLOR_BLUE "Alignas Node: " COLOR_RESET "\n");
    indent_level++;
    print_indent();
    printf("Expression: \n");
    indent_level++;
    alignas_node->expr->print(alignas_node->expr);
    indent_level--;
    indent_level--;
}
