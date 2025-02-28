typedef enum NodeType {
    IDENTIFIER_NODE,
    CONSTANT_NODE,
    BINARY_OPERATION_NODE,
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

Node *create_identifier_node(char* name);
Node *create_constant_int_node(int value);
Node *create_constant_float_node(float value);
Node *create_binary_operation_node(int op, Node* left, Node* right);
void print_identifier_node(Node* node);
void print_const_node(Node* node);
void print_binary_operation_node(Node* node);