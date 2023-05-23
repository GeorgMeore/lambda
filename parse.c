#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#include "iter.h"


static Expr *parse_application(CharIterator *iter);
static Expr *parse_term(CharIterator *iter);
static Expr *parse_group(CharIterator *iter);

// EXPR ::= APPLICATION '\0'
Expr *parse(CharIterator *iter)
{
	Expr *expr = parse_application(iter);
	if (!expr) {
		return NULL;
	}
	if (CharIterator_peek(iter) != 0) {
		fprintf(stderr, "error: unexpected character: '%c'\n", CharIterator_peek(iter));
		Expr_drop(expr);
		return NULL;
	}
	return expr;
}

// TERM | APPLICATION TERM
static Expr *parse_application(CharIterator *iter)
{
	Expr *left = parse_term(iter);
	if (!left) {
		return NULL;
	}
	while (isalpha(CharIterator_peek(iter)) || CharIterator_peek(iter) == '(') {
		Expr *right = parse_term(iter);
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
static Expr *parse_term(CharIterator *iter)
{
	char next = CharIterator_peek(iter);
	if (next == '(') {
		return parse_group(iter);
	}
	if (isalpha(next)) {
		Expr *expr = Var_new(CharIterator_next(iter));
		if (CharIterator_peek(iter) == '.') {
			CharIterator_next(iter);
			Expr *body = parse_application(iter);
			if (!body) {
				Expr_drop(expr);
				return NULL;
			}
			expr = Lambda_new(expr, body);
		}
		return expr;
	}
	fprintf(stderr, "error: unexpected character: '%c'\n", next);
	return NULL;
}

// GROUP ::= '(' APPLICATION ')'
static Expr *parse_group(CharIterator *iter)
{
	CharIterator_next(iter);
	Expr *expr = parse_application(iter);
	if (!expr) {
		return NULL;
	}
	if (CharIterator_next(iter) != ')') {
		fprintf(stderr, "error: ')' expected\n");
		Expr_drop(expr);
		return NULL;
	}
	return expr;
}
