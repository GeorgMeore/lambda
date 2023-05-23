#include "reduce.h"

#include <stdio.h>

#include "expr.h"


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
