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
		Expr_print(expr->as.appl.right);
		break;
	case LAMBDA_EXPR:
		printf("(\\%d.", expr->as.lambda.arg);
		Expr_print(expr->as.lambda.body);
		printf(")");
		break;
	}
}

void Expr_drop(Expr *expr)
{
	switch (expr->type) {
	case APPL_EXPR:
		Expr_drop(expr->as.appl.left);
		Expr_drop(expr->as.appl.right);
		break;
	case LAMBDA_EXPR:
		Expr_drop(expr->as.lambda.body);
		break;
	case FREE_VAR_EXPR: case BOUND_VAR_EXPR:
		// noop
	}
	free(expr);
}


static void sub_free_variable(Expr *expr, int var)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		if (expr->as.var == var) {
			expr->type = BOUND_VAR_EXPR;
			expr->as.var = 0;
		}
		break;
	case APPL_EXPR:
		sub_free_variable(expr->as.appl.left, var);
		sub_free_variable(expr->as.appl.right, var);
		break;
	case LAMBDA_EXPR:
		sub_free_variable(expr->as.lambda.body, var);
		break;
	case BOUND_VAR_EXPR:
		// noop
	}
}

static void inc_lambda_args(Expr *expr)
{
	switch (expr->type) {
	case BOUND_VAR_EXPR:
		expr->as.var += 1;
		break;
	case APPL_EXPR:
		inc_lambda_args(expr->as.appl.left);
		inc_lambda_args(expr->as.appl.right);
		break;
	case LAMBDA_EXPR:
		expr->as.lambda.arg += 1;
		inc_lambda_args(expr->as.lambda.body);
		break;
	case FREE_VAR_EXPR:
		// noop
	}
}

Expr *Lambda_new(Expr *arg, Expr *body)
{
	Expr *lambda = malloc(sizeof(*lambda));
	lambda->type = LAMBDA_EXPR;
	sub_free_variable(body, arg->as.var);
	Expr_drop(arg);
	inc_lambda_args(body);
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
