#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#include "scanner.h"
#include "node.h"
#include "arena.h"


// subexpression parsers
static Node *parse_definition(Arena *a, Scanner *scanner);
static Node *parse_application(Arena *a, Scanner *scanner);
static Node *parse_lambda(Arena *a, Scanner *scanner);
static Node *parse_term(Arena *a, Scanner *scanner);
static Node *parse_group(Arena *a, Scanner *scanner);

// AST ::= (APPLICATION | DEFINITION) '\0'
Node *parse(Arena *a, Scanner *scanner)
{
	Node *node = NULL;
	if (isupper(Scanner_peek(scanner)) && Scanner_peek2(scanner) == '=') {
		node = parse_definition(a, scanner);
	} else {
		node = parse_application(a, scanner);
	}
	if (!node) {
		return NULL;
	}
	if (Scanner_peek(scanner) != 0) {
		fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
		return NULL;
	}
	Scanner_next(scanner);
	return node;
}

// DEFINITION ::= NAME '=' APPLICATION
Node *parse_definition(Arena *a, Scanner *scanner)
{
	Node *name = Node_new(a, NAME_NODE, Scanner_next(scanner));
	Scanner_next(scanner); // skip '='
	Node *value = parse_application(a, scanner);
	if (!value) {
		return NULL;
	}
	return Node_new(a, DEF_NODE, name, value);
}

// APPLICATION ::= TERM | APPLICATION TERM
static Node *parse_application(Arena *a, Scanner *scanner)
{
	Node *left = parse_term(a, scanner);
	if (!left) {
		return NULL;
	}
	while (isalpha(Scanner_peek(scanner)) || Scanner_peek(scanner) == '(') {
		Node *right = parse_term(a, scanner);
		if (!right) {
			return NULL;
		}
		left = Node_new(a, APPL_NODE, left, right);
	}
	return left;
}

// TERM ::= GROUP | NAME | VAR | LAMBDA
static Node *parse_term(Arena *a, Scanner *scanner)
{
	if (Scanner_peek(scanner) == '(') {
		return parse_group(a, scanner);
	}
	if (isupper(Scanner_peek(scanner))) {
		return Node_new(a, NAME_NODE, Scanner_next(scanner));
	}
	if (islower(Scanner_peek(scanner))) {
		if (Scanner_peek2(scanner) == '.') {
			return parse_lambda(a, scanner);
		}
		return Node_new(a, VAR_NODE, Scanner_next(scanner));
	}
	fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
	return NULL;
}

// LAMBDA ::= VAR '.' APPLICATION
Node *parse_lambda(Arena *a, Scanner *scanner)
{
	Node *var = Node_new(a, VAR_NODE, Scanner_next(scanner));
	Scanner_next(scanner); // skip '.'
	Node *body = parse_application(a, scanner);
	if (!body) {
		return NULL;
	}
	return Node_new(a, LAMBDA_NODE, var, body);
}

// GROUP ::= '(' APPLICATION ')'
static Node *parse_group(Arena *a, Scanner *scanner)
{
	Scanner_next(scanner);
	Node *node = parse_application(a, scanner);
	if (!node) {
		return NULL;
	}
	if (Scanner_next(scanner) != ')') {
		fprintf(stderr, "error: ')' expected\n");
		return NULL;
	}
	return node;
}
