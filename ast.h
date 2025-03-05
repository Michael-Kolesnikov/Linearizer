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
    int op;
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

Node *create_identifier_node(char* name);
Node *create_constant_int_node(int value);
Node *create_constant_float_node(float value);
Node *create_binary_operation_node(int op, Node* left, Node* right);
Node *create_declaration_node(Node* identifier, Node* initializer);
Node* create_assignment_node(Node* left, char* op, Node* right);
Node* create_logical_operation_node(Node* left, char* op, Node* right);
Node* create_if_node(Node* condition, Node* then_statement, Node* else_statement);
Node* create_expression_statement_node(Node* expr);
Node* create_empty_statement_node();
void print_identifier_node(Node* node);
void print_const_node(Node* node);
void print_binary_operation_node(Node* node);
void print_declaration_node(Node* node);
void print_assignment_node(Node* node);
void print_logical_operation_node(Node* node);
void print_if_node(Node* node);
void print_expression_statement_node(Node* node);
void print_empty_statement_node(Node* node);