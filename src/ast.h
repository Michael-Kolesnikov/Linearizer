#ifndef AST_H
#define AST_H
typedef enum NodeType {
    IDENTIFIER_NODE,
    CONSTANT_NODE,
    BINARY_OPERATION_NODE,
    DECLARATION_NODE,
    ASSIGNMENT_NODE,
    LOGICAL_OPERATION_NODE,
    IF_NODE,
    EXPRESSION_STATEMENT_NODE,
    EMPTY_STATEMENT_NODE,
    COMPOUND_STATEMENT_NODE,
    FUNCTION_DECLARATION_NODE,
    STRING_LITERAL_NODE,
    WHILE_NODE,
    DOWHILE_NODE,
    SWITCH_NODE,
    CASE_NODE,
    DEFAULT_NODE,
    BREAK_NODE,
    TERNARY_OPERATOR_NODE,
    RETURN_NODE,
    GOTO_NODE,
    CONTINUE_NODE,
    PREFIX_INCREMENT_NODE,
    POSTFIX_INCEMENT_NODE,
    PREFIX_DECREMENT_NODE,
    POSTFIX_DECREMENT_NODE,
    UNARY_OPERATOR_EXPRESSION_NODE,
    POINTER_NODE,
    PARAMETERS_NODE,
    FUNCTION_DECLARATOR_NODE,
    FOR_NODE,
    ARGUMENTS_NODE,
    FUNCTION_CALL_NODE,
    ARRAY_DECLARATION_NODE,
    ARRAY_EXPRESSION_NODE,
    SIZEOF_NODE,
    VALUE_NODE,
    MEMBER_ACCESS_EXPRESSION_NODE,
    POINTER_MEMBER_ACCESS_EXPRESSION_NODE,
    STRUCTUNION_NODE,
    STRUCT_DECLARATOR_NODE,
    STRUCT_DECLARATIONS_LIST_NODE,
    CAST_EXPRESSION_NODE,
    LABELED_STATEMENT_NODE,
    WRAPPER_NODE,
    ENUM_NODE,
    ENUMERATORS_LIST_NODE,
    MEMBER_DESIGNATOR_NODE,
    ARRAY_DESIGNATOR_NODE,
    DESIGNATORS_LIST_NODE,
    INITIALIZER_NODE,
    INITIALIZERS_LIST_NODE,
    DECLARATORS_LIST_NODE,
    GROUPED_DECLARATOR_NODE,
    COMPOUND_LITERALS_NODE,
    PARENTHESIZED_EXPRESSION_NODE,
    STATIC_ASSERT_NODE,
} NodeType;

typedef struct Node{
    NodeType type;
    void (*print)(struct Node*);
} Node;

typedef struct IdentifierNode{
    Node base;
    char* name;
} IdentifierNode;

typedef struct ConstantNode{
    Node base;
    enum {
        CONST_INT,
        CONST_FLOAT
    } const_type;
    union{
        int i_value;
        float f_value;
    } value;
} ConstantNode;

typedef struct BinaryOperationNode{
    Node base;
    Node* left;
    Node* right;
    char* op;
} BinaryOperationNode;

typedef struct {
    Node base;
    Node* identifier;
    Node* initializer;
} DeclarationNode;

typedef struct {
    Node base;
    Node* left;
    char*  op;
    Node* right;
} AssignmentNode;

typedef struct {
    Node base;
    Node* left;
    char* op;
    Node* right;
} LogicalOperationNode;

typedef struct {
    Node base;
    Node* condition;
    Node* then_statement;
    Node* else_statement;
} IfNode;

typedef struct {
    Node base;
    Node* expr;
} ExpressionStatementNode;

typedef struct {
    Node base;
} EmptyStatementNode;

typedef struct {
    Node base;
    Node** statements;
    int count;
} CompoundStatementNode;

typedef struct {
    Node base;
    Node* return_type;
    Node* declarator;
    Node* body;
} FunctionDeclarationNode;

typedef struct {
    Node base;
    char* value;
} StringNode;

typedef struct {
    Node base;
    Node* condition;
    Node* body;
} WhileNode;

typedef struct {
    Node base;
    Node* do_statement;
    Node* condition;
} DoWhileNode;

typedef struct {
    Node base;
    Node* expression;
    Node* body;
} SwitchNode;

typedef struct {
    Node base;
    Node* expression;
    Node* body;
} CaseNode;

typedef struct {
    Node base;
    Node* body;
} DefaultNode;

typedef struct {
    Node base;
} BreakNode;

typedef struct {
    Node base;
    Node* condition;
    Node* then_statement;
    Node* else_statement;
} TernaryOperatorNode;

typedef struct {
    Node base;
    Node* expression;
} ReturnNode;

typedef struct {
    Node base;
    Node* identifier;
} GotoNode;

typedef struct {
    Node base;
    Node* expression;
} ContinueNode;

typedef struct {
    Node base;
    Node* expression;
} PrefixIncrement;

typedef struct {
    Node base;
    Node* expression;
} PostfixIncrement;

typedef struct {
    Node base;
    Node* expression;
} PrefixDecrement;

typedef struct {
    Node base;
    Node* expression;
} PostfixDecrement;

typedef struct {
    Node base;
    Node* expression;
    char* unary_operator;
} UnaryOperatorExpressonNode;

typedef struct{
    Node base;
    Node* point;
    Node* declarator;
} PointerNode;

typedef struct{
    Node base;
    Node** parameters;
    int count;
} ParametersNode;

typedef struct{
    Node base;
    Node* declarator;
    Node* parameters;
} FunctionDeclaratorNode;

typedef struct{
    Node base;
    Node* initialization;
    Node* condition;
    Node* update;
    Node* body;
} ForNode;

typedef struct {
    Node base;
    Node** arguments;
    int count;
} ArgumentsNode;

typedef struct {
    Node base;
    Node* name;
    Node* arguments;
} FunctionCallNode;

typedef struct {
    Node base;
    Node* declarator;
    Node* index_expression;
} ArrayDeclarationNode;

typedef struct {
    Node base;
    Node* identifier;
    Node* index;
} ArrayExpressionNode;

typedef struct {
    Node base;
    Node* expression;
} SizeofNode;

typedef struct {
    Node base;
    char* value;
} ValueNode;

typedef struct {
    Node base;
    Node* object_expression;
    Node* field_name;
} MemberAccessExpressionNode;

typedef struct {
    Node base;
    Node* pointer_expression;
    Node* field_name;
} PointerMemberAccessExpressionNode;

typedef struct {
    Node base;
    Node* kind;
    Node* identifier;
    Node* body;
} StructUnionNode;

typedef struct {
    Node base;
    Node* declarator;
    Node* bit_width;
} StructDeclaratorNode;

typedef struct {
    Node base;
    Node** declarations_list;
    int count;
} StructDeclarationsListNode;

typedef struct {
    Node base;
    Node* type;
    Node* expression;
} CastExpressionNode;

typedef struct {
    Node base;
    Node* identifier;
    Node* statement;
} LabeledStatementNode;

typedef struct {
    Node base;
    Node* wrapper;
    Node* inner_node;
} WrapperNode;

typedef struct {
    Node base;
    Node* identifier;
    Node* enumerators_list;
} EnumNode;

typedef struct {
    Node base;
    Node** enumerators_list;
    int count;
} EnumeratorsListNode;

typedef struct {
    Node base;
    Node* identifier;

} MemberDesignatorNode;

typedef struct {
    Node base;
    Node* expression;

} ArrayDesignatorNode;

typedef struct {
    Node base;
    Node** designators;
    int count;
} DesignatorsListNode;

typedef struct {
    Node base;
    Node* initializer;
    Node* designation;

} InitializerNode;

typedef struct {
    Node base;
    Node** initializers;
    int count;
} InitializersListNode;

typedef struct {
    Node base;
    Node** declarators;
    Node* type_specifier;
    int count;
} DeclaratorsListNode;

typedef struct {
    Node base;
    Node* declarator;
} GroupedDeclaratorNode;

typedef struct {
    Node base;
    Node* initializer_list;
    Node* type_name;
} CompoundLiteralsNode;

typedef struct {
    Node base;
    Node* expression;
} ParenthesizedExpressionNode;

typedef struct {
    Node base;
    Node* expr;
    Node* message;
} StaticAssertNode;

Node *create_identifier_node(char* name);
Node *create_constant_int_node(int value);
Node *create_constant_float_node(float value);
Node *create_binary_operation_node(char* op, Node* left, Node* right);
Node *create_declaration_node(Node* identifier, Node* initializer);
Node* create_assignment_node(Node* left, char* op, Node* right);
Node* create_logical_operation_node(Node* left, char* op, Node* right);
Node* create_if_node(Node* condition, Node* then_statement, Node* else_statement);
Node* create_expression_statement_node(Node* expr);
Node* create_empty_statement_node();
Node* create_compound_statement_node(Node** statement, int count);
Node* create_function_declaration_node(Node* return_type, Node* declarator, Node* body);
Node* create_string_literal_node(char* value);
Node* create_while_node(Node* condition, Node* body);
Node* create_do_while_node(Node* do_statement, Node* condition);
Node* create_switch_node(Node* expression, Node* body);
Node* create_case_node(Node* expression, Node* body);
Node* create_default_node(Node* body);
Node* create_break_node();
Node* create_ternary_operator_node(Node* condition, Node* then_statement, Node* else_statement);
Node* create_return_node(Node* expression);
Node* create_goto_node(Node* identifier);
Node* create_continue_node();
Node* create_prefix_increment_node(Node* expression);
Node* create_postfix_increment_node(Node* expression);
Node* create_prefix_decrement_node(Node* expression);
Node* create_postfix_decrement_node(Node* expression);
Node* create_unary_operator_expression_node(char* unary_operator, Node* expression);
Node* create_pointer_node(Node* point, Node* declarator);
Node* create_parameters_node(Node** parameters, int count);
Node* create_function_declarator_node(Node* declarator, Node* parameters);
Node* create_for_node(Node* initialization, Node* condition, Node* update, Node* body);
Node* create_arguments_node(Node** arguments, int count);
Node* create_function_call_node(Node* name, Node* arguments);
Node* create_array_declaration_node(Node* declarator, Node* index_expression);
Node* create_array_expression_node(Node* identifier, Node* index);
Node* create_sizeof_node(Node* expression);
Node* create_value_node(char* value);
Node* create_member_access_expression_node(Node* object_expression, Node* field_name);
Node* create_pointer__member_access_expression_node(Node* pointer_expression, Node* field_name);
Node* create_structunion_node(Node* kind, Node* identifier, Node* body);
Node* create_struct_declarator_node(Node* declarator, Node* bit_width);
Node* create_struct_declarations_list_node(Node** declarations, int count);
Node* create_cast_expression_node(Node* type, Node* expression);
Node* create_labeled_statement_node(Node* identifier, Node* statement);
Node* create_wrapper_node(Node* wrapper, Node* inner_node);
Node* create_enum_node(Node* identifier, Node* enumerators_list);
Node* create_enumerators_list_node(Node** enumerators_list, int count);
Node* create_member_designator_node(Node* identifier);
Node* create_array_designator_node(Node* expression);
Node* create_designator_list_node(Node** designators, int count);
Node* create_initializer_node(Node* initializer, Node* desgnation);
Node* create_initializers_list_node(Node** initializers, int count);
Node* create_declarators_list_node(Node** declarators, int count);
Node* create_grouped_declarator_node(Node* declarator);
Node* create_compound_literals_node(Node* initializer_list, Node* type_name);
Node* create_parenthesized_expression_node(Node* expression);
Node* create_static_assert_node(Node* expr, Node* message);
void print_identifier_node(Node* node);
void print_const_node(Node* node);
void print_binary_operation_node(Node* node);
void print_declaration_node(Node* node);
void print_assignment_node(Node* node);
void print_logical_operation_node(Node* node);
void print_if_node(Node* node);
void print_expression_statement_node(Node* node);
void print_empty_statement_node(Node* node);
void print_compound_statement_node(Node* node);
void print_function_declaration_node(Node* node);
void print_string_literal_node(Node* node);
void print_while_node(Node* node);
void print_do_while_node(Node* node);
void print_switch_node(Node* node);
void print_case_node(Node* node);
void print_default_node(Node* node);
void print_break_node(Node* node);
void print_ternary_operator_node(Node* node);
void print_return_node(Node* node);
void print_goto_node(Node* node);
void print_continue_node(Node* node);
void print_prefix_increment_node(Node* node);
void print_postfix_increment_node(Node* node);
void print_prefix_decrement_node(Node* node);
void print_postfix_decrement_node(Node* node);
void print_unary_operator_expression_node(Node* node);
void print_pointer_node(Node* node);
void print_parameters_node(Node* node);
void print_function_declarator_node(Node* node);
void print_for_node(Node* node);
void print_arguments_node(Node* node);
void print_function_call_node(Node* node);
void print_array_declaration_node(Node* node);
void print_array_expression_node(Node* node);
void print_sizeof_node(Node* node);
void print_value_node(Node* node);
void print_member_access_expression_node(Node* node);
void print_pointer_member_access_expression_node(Node* node);
void print_structunion_node(Node* node);
void print_struct_declarator_node(Node* node);
void print_struct_declarations_list_node(Node* node);
void print_cast_expression_node(Node* node);
void print_labeled_statement_node(Node* node);
void print_wrapper_node(Node* node);
void print_enum_node(Node* node);
void print_enumerators_list_node(Node* node);
void print_member_designator_node(Node* node);
void print_array_designator_node(Node* node);
void print_designators_list_node(Node* node);
void print_initializer_node(Node* node);
void print_initializers_list_node(Node* node);
void print_declarators_list_node(Node* node);
void print_grouped_declarator_node(Node* node);
void print_compound_literals_node(Node* node);
void print_parenthesized_expression_node(Node* node);
void print_static_assert_node(Node* node);
#endif // AST_H