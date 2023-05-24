#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void Node_print(Node *node)
{
	switch (node->type) {
	case VAR_NODE:
		printf("%c", node->as.var);
		break;
	case NAME_NODE:
		printf("%c", node->as.name);
		break;
	case APPL_NODE:
		Node_print(node->as.appl.left);
		if (node->as.appl.right->type == APPL_NODE) {
			putchar('(');
			Node_print(node->as.appl.right);
			putchar(')');
		} else {
			Node_print(node->as.appl.right);
		}
		break;
	case LAMBDA_NODE:
		putchar('(');
		Node_print(node->as.lambda.arg);
		putchar('.');
		Node_print(node->as.lambda.body);
		putchar(')');
		break;
	case DEF_NODE:
		Node_print(node->as.def.name);
		putchar('=');
		Node_print(node->as.def.value);
		break;
	}
}

void Node_println(Node *node)
{
	Node_print(node);
	putchar('\n');
}

void Node_drop(Node *node)
{
	switch (node->type) {
	case VAR_NODE:
	case NAME_NODE:
		break;
	case APPL_NODE:
		Node_drop(node->as.appl.left);
		Node_drop(node->as.appl.right);
		break;
	case LAMBDA_NODE:
		Node_drop(node->as.lambda.arg);
		Node_drop(node->as.lambda.body);
		break;
	case DEF_NODE:
		Node_drop(node->as.def.name);
		Node_drop(node->as.def.value);
		break;
	}
	free(node);
}

Node *Node_new(NodeType type, ...)
{
	Node *node = malloc(sizeof(*node));
	va_list args;
	va_start(args, type);
	node->type = type;
	switch (type) {
	case VAR_NODE:
		node->as.var = va_arg(args, unsigned);
		break;
	case NAME_NODE:
		node->as.name = va_arg(args, unsigned);
		break;
	case APPL_NODE:
		node->as.appl.left = va_arg(args, Node*);
		node->as.appl.right = va_arg(args, Node*);
		break;
	case LAMBDA_NODE:
		node->as.lambda.arg = va_arg(args, Node*);
		node->as.lambda.body = va_arg(args, Node*);
		break;
	case DEF_NODE:
		node->as.def.name = va_arg(args, Node*);
		node->as.def.value = va_arg(args, Node*);
		break;
	}
	return node;
}
