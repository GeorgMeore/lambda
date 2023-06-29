#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include "arena.h"


typedef enum {
	LAMBDA_NODE,
	APPL_NODE,
	VAR_NODE,
	DEF_NODE,
	NAME_NODE,
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

typedef unsigned NameValue;

typedef union {
	VarValue var;
	NameValue name;
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
Node* Node_new(Arena *a, NodeType type, ...);

#endif // NODE_INCLUDED
