#ifndef NODE_INCLUDED
#define NODE_INCLUDED

typedef enum {
	LAMBDA_NODE,
	APPL_NODE,
	VAR_NODE,
	DEF_NODE,
} NodeType;

typedef struct Node Node;

typedef struct {
	Node *arg;
	Node *body;
} LambdaValue;

typedef struct {
	Node *name;
	Node *value;
} DefValue;

typedef struct {
	Node *left;
	Node *right;
} ApplValue;

typedef unsigned VarValue;

typedef union {
	VarValue var;
	LambdaValue lambda;
	ApplValue appl;
	DefValue def;
} NodeValue;

struct Node {
	NodeType type;
	NodeValue as;
};

void  Node_print(Node *node);
void  Node_println(Node *node);
Node* Node_new(NodeType type, ...);
void  Node_drop(Node *node);

#endif // NODE_INCLUDED
