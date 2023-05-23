#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#include "scanner.h"


// subexpression parsers
static Expr *parse_application(Scanner *scanner);
static Expr *parse_term(Scanner *scanner);
static Expr *parse_group(Scanner *scanner);

// EXPR ::= APPLICATION '\0'
Expr *parse(Scanner *scanner)
{
	Expr *expr = parse_application(scanner);
	if (!expr) {
		return NULL;
	}
	if (Scanner_peek(scanner) != 0) {
		fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
		Expr_drop(expr);
		return NULL;
	}
	return expr;
}

// TERM | APPLICATION TERM
static Expr *parse_application(Scanner *scanner)
{
	Expr *left = parse_term(scanner);
	if (!left) {
		return NULL;
	}
	while (isalpha(Scanner_peek(scanner)) || Scanner_peek(scanner) == '(') {
		Expr *right = parse_term(scanner);
		if (!right) {
			Expr_drop(left);
			return NULL;
		}
		left = Appl_new(left, right);
	}
	return left;
}

// TERM   ::= GROUP | LAMBDA | VAR
// LAMBDA ::= VAR '.' APPLICATION
static Expr *parse_term(Scanner *scanner)
{
	if (Scanner_peek(scanner) == '(') {
		return parse_group(scanner);
	}
	if (isalpha(Scanner_peek(scanner))) {
		Expr *expr = Var_new(Scanner_next(scanner));
		if (Scanner_peek(scanner) == '.') {
			Scanner_next(scanner);
			Expr *body = parse_application(scanner);
			if (!body) {
				Expr_drop(expr);
				return NULL;
			}
			expr = Lambda_new(expr, body);
		}
		return expr;
	}
	fprintf(stderr, "error: unexpected character: '%c'\n", Scanner_peek(scanner));
	return NULL;
}

// GROUP ::= '(' APPLICATION ')'
static Expr *parse_group(Scanner *scanner)
{
	Scanner_next(scanner);
	Expr *expr = parse_application(scanner);
	if (!expr) {
		return NULL;
	}
	if (Scanner_next(scanner) != ')') {
		fprintf(stderr, "error: ')' expected\n");
		Expr_drop(expr);
		return NULL;
	}
	return expr;
}
