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
    char* type_specifier;
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
    char* return_type;
    Node* name;
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
    Node* declarator;
} PointerNode;
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
Node* create_function_declaration_node(char* return_type, Node* name, Node* body);
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
Node* create_pointer_node(Node* declarator);
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
#endif // AST_H