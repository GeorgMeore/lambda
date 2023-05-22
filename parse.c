#include "parse.h"

#include <stdio.h>
#include <ctype.h>

#include "iter.h"


static Expr *parse_application(CharIterator *iter);
static Expr *parse_term(CharIterator *iter);
static Expr *parse_group(CharIterator *iter);
static Expr *parse_lambda(CharIterator *iter);

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

static int is_term_char(char c)
{
	return c == '(' || c == '\\' || isalpha(c);
}

// TERM | APPLICATION TERM
static Expr *parse_application(CharIterator *iter)
{
	Expr *left = parse_term(iter);
	if (!left) {
		return NULL;
	}
	while (is_term_char(CharIterator_peek(iter))) {
		Expr *right = parse_term(iter);
		if (!right) {
			Expr_drop(left);
			return NULL;
		}
		left = Appl_new(left, right);
	}
	return left;
}

// TERM ::= GROUP | LAMBDA | VAR
static Expr *parse_term(CharIterator *iter)
{
	char next = CharIterator_peek(iter);
	if (next == '(') {
		return parse_group(iter);
	}
	if (next == '\\') {
		return parse_lambda(iter);
	}
	if (isalpha(next)) {
		CharIterator_next(iter);
		return Var_new(next);
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

// LAMBDA ::= '\' VAR '.' APPLICATION
static Expr *parse_lambda(CharIterator *iter)
{
	CharIterator_next(iter);
	if (!isalpha(CharIterator_peek(iter))) {
		fprintf(stderr, "error: variable expected\n");
		return NULL;
	}
	Expr *arg = Var_new(CharIterator_next(iter));
	if (CharIterator_next(iter) != '.') {
		fprintf(stderr, "error: '.' expected\n");
		Expr_drop(arg);
		return NULL;
	}
	Expr *body = parse_application(iter);
	if (!body) {
		Expr_drop(arg);
		return NULL;
	}
	return Lambda_new(arg, body);
}
