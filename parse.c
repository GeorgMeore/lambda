#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#include "scanner.h"
#include "node.h"


// subexpression parsers
static Node *parse_definition(Scanner *scanner);
static Node *parse_application(Scanner *scanner);
static Node *parse_lambda(Scanner *scanner);
static Node *parse_term(Scanner *scanner);
static Node *parse_group(Scanner *scanner);

// AST ::= (APPLICATION | DEFINITION) '\0'
Node *parse(Scanner *scanner)
{
	Node *node = NULL;
	if (isupper(Scanner_peek(scanner)) && Scanner_peeek(scanner) == '=') {
		node = parse_definition(scanner);
	} else {
		node = parse_application(scanner);
	}
	if (!node) {
		return NULL;
	}
	if (Scanner_peek(scanner) != 0) {
		fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
		Node_drop(node);
		return NULL;
	}
	return node;
}

// DEFINITION ::= NAME '=' APPLICATION
Node *parse_definition(Scanner *scanner)
{
	Node *name = Node_new(NAME_NODE, Scanner_next(scanner));
	Scanner_next(scanner); // skip '='
	Node *value = parse_application(scanner);
	if (!value) {
		Node_drop(name);
		return NULL;
	}
	return Node_new(DEF_NODE, name, value);
}

// APPLICATION ::= TERM | APPLICATION TERM
static Node *parse_application(Scanner *scanner)
{
	Node *left = parse_term(scanner);
	if (!left) {
		return NULL;
	}
	while (isalpha(Scanner_peek(scanner)) || Scanner_peek(scanner) == '(') {
		Node *right = parse_term(scanner);
		if (!right) {
			Node_drop(left);
			return NULL;
		}
		left = Node_new(APPL_NODE, left, right);
	}
	return left;
}

// TERM ::= GROUP | NAME | VAR | LAMBDA
static Node *parse_term(Scanner *scanner)
{
	if (Scanner_peek(scanner) == '(') {
		return parse_group(scanner);
	}
	if (isupper(Scanner_peek(scanner))) {
		return Node_new(NAME_NODE, Scanner_next(scanner));
	}
	if (islower(Scanner_peek(scanner))) {
		if (Scanner_peeek(scanner) == '.') {
			return parse_lambda(scanner);
		}
		return Node_new(VAR_NODE, Scanner_next(scanner));
	}
	fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
	return NULL;
}

// LAMBDA ::= VAR '.' APPLICATION
Node *parse_lambda(Scanner *scanner)
{
		Node *var = Node_new(VAR_NODE, Scanner_next(scanner));
		Scanner_next(scanner); // skip '.'
		Node *body = parse_application(scanner);
		if (!body) {
			Node_drop(var);
			return NULL;
		}
		return Node_new(LAMBDA_NODE, var, body);
}

// GROUP ::= '(' APPLICATION ')'
static Node *parse_group(Scanner *scanner)
{
	Scanner_next(scanner);
	Node *node = parse_application(scanner);
	if (!node) {
		return NULL;
	}
	if (Scanner_next(scanner) != ')') {
		fprintf(stderr, "error: ')' expected\n");
		Node_drop(node);
		return NULL;
	}
	return node;
}
