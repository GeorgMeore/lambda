#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "scanner.h"
#include "parse.h"
#include "node.h"
#include "expr.h"
#include "table.h"


// normal order reduction
int reduce(Expr *expr)
{
	if (expr->type == LAMBDA_EXPR) {
		return reduce(expr->as.lambda.body);
	}
	if (expr->type == APPL_EXPR) {
		Expr *left = expr->as.appl.left, *right = expr->as.appl.right;
		if (left->type != LAMBDA_EXPR) {
			return reduce(left) || reduce(right);
		}
		Expr_beta_reduce(expr);
		return 1;
	}
	return 0;
}

Expr *analyze(Node *node, Table bindings)
{
	switch (node->type) {
	case LAMBDA_NODE:
		Expr *arg = analyze(node->as.lambda.arg, bindings);
		if (!arg) {
			return NULL;
		}
		Expr *body = analyze(node->as.lambda.body, bindings);
		if (!body) {
			Expr_drop(arg);
			return NULL;
		}
		return Lambda_new(arg, body );
	case APPL_NODE:
		Expr *left = analyze(node->as.appl.left, bindings);
		if (!left) {
			return NULL;
		}
		Expr *right = analyze(node->as.appl.right, bindings);
		if (!right) {
			Expr_drop(left);
			return NULL;
		}
		return Appl_new(left, right);
	case VAR_NODE:
		return Var_new(node->as.var);
	case NAME_NODE:
		unsigned name = node->as.name;
		if (!Table_has(bindings, name)) {
			fprintf(stderr, "error: unbound name '%c'\n", name);
			return NULL;
		}
		return Expr_copy(Table_lookup(bindings, name));
	case DEF_NODE: // this cannot happen, but why not
		fprintf(stderr, "error: cannot evaluate definition\n");
	}
	return NULL;
}

Expr *evaluate(Node *ast, Table bindings)
{
	if (ast->type == DEF_NODE) {
		unsigned name = ast->as.def.name->as.name;
		if (Table_has(bindings, name)) {
			Expr_drop(Table_lookup(bindings, name));
		}
		Expr *value = analyze(ast->as.def.value, bindings);
		Table_set(bindings, name, value);
		return NULL;
	} else {
		return analyze(ast, bindings);
	}
}

int main()
{
	int tty = isatty(1);
	Table bindings = {0};
	for (;;) {
		if (tty) {
			fprintf(stderr, "> ");
		}
		Scanner scanner = get_line();
		if (!scanner) {
			break;
		}
		Node *ast = parse(&scanner);
		if (!ast) {
			continue;
		}
		Expr *expr = evaluate(ast, bindings);
		Node_drop(ast);
		if (!expr) {
			continue;
		}
		for (;;) {
			if (tty) {
				fprintf(stderr, "- ");
			}
			Expr_println(expr);
			if (!reduce(expr)) {
				break;
			}
		}
		Expr_drop(expr);
	}
	Table_clear(bindings);
	return 0;
}
