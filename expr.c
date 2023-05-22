#include "expr.h"

#include <stdio.h>
#include <stdlib.h>


void Expr_print(Expr *expr)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		printf("%c", expr->as.var);
		break;
	case BOUND_VAR_EXPR:
		printf("%d", expr->as.var);
		break;
	case APPL_EXPR:
		Expr_print(expr->as.appl.left);
		if (expr->as.appl.right->type == APPL_EXPR) {
			putchar('(');
			Expr_print(expr->as.appl.right);
			putchar(')');
		} else {
			Expr_print(expr->as.appl.right);
		}
		break;
	case LAMBDA_EXPR:
		printf("(\\%d.", expr->as.lambda.arg);
		Expr_print(expr->as.lambda.body);
		putchar(')');
		break;
	}
}

void Expr_drop(Expr *expr)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
	case BOUND_VAR_EXPR:
		free(expr);
		break;
	case APPL_EXPR:
		Expr_drop(expr->as.appl.left);
		Expr_drop(expr->as.appl.right);
		free(expr);
		break;
	case LAMBDA_EXPR:
		Expr_drop(expr->as.lambda.body);
		free(expr);
		break;
	}
}

static void sub_free_variable(Expr *expr, int var)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		if (expr->as.var == var) {
			expr->type = BOUND_VAR_EXPR;
			expr->as.var = 1;
		}
		break;
	case BOUND_VAR_EXPR:
		expr->as.var += 1;
		break;
	case APPL_EXPR:
		sub_free_variable(expr->as.appl.left, var);
		sub_free_variable(expr->as.appl.right, var);
		break;
	case LAMBDA_EXPR:
		expr->as.lambda.arg += 1;
		sub_free_variable(expr->as.lambda.body, var);
		break;
	}
}

Expr *Lambda_new(Expr *arg, Expr *body)
{
	Expr *lambda = malloc(sizeof(*lambda));
	lambda->type = LAMBDA_EXPR;
	sub_free_variable(body, arg->as.var);
	Expr_drop(arg);
	lambda->as.lambda.arg = 1;
	lambda->as.lambda.body = body;
	return lambda;
}

Expr *Appl_new(Expr *left, Expr *right)
{
	Expr *appl = malloc(sizeof(*appl));
	appl->type = APPL_EXPR;
	appl->as.appl.left = left;
	appl->as.appl.right = right;
	return appl;
}

Expr *Var_new(int variable)
{
	Expr *var = malloc(sizeof(*var));
	var->type = FREE_VAR_EXPR;
	var->as.var = variable;
	return var;
}
