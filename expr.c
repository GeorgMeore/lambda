#include "expr.h"

#include <stdio.h>
#include <stdlib.h>


// TODO: more readable printout format
void Expr_print(Expr *expr)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		printf("%c", expr->as.var);
		break;
	case BOUND_VAR_EXPR:
		printf("v%d", expr->as.var);
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
		printf("(v%d.", expr->as.lambda.arg);
		Expr_print(expr->as.lambda.body);
		putchar(')');
		break;
	}
}

void Expr_println(Expr *expr)
{
	Expr_print(expr);
	putchar('\n');
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

static void Expr_inc_args(Expr *expr, unsigned cutoff)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		break;
	case BOUND_VAR_EXPR:
		if (expr->as.var >= cutoff) {
			expr->as.var += 1;
		}
		break;
	case APPL_EXPR:
		Expr_inc_args(expr->as.appl.left, cutoff);
		Expr_inc_args(expr->as.appl.right, cutoff);
		break;
	case LAMBDA_EXPR:
		expr->as.lambda.arg += 1;
		Expr_inc_args(expr->as.lambda.body, cutoff);
		break;
	}
}

static void Expr_dec_args(Expr *expr, unsigned cutoff)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		break;
	case BOUND_VAR_EXPR:
		if (expr->as.var >= cutoff) {
			expr->as.var -= 1;
		}
		break;
	case APPL_EXPR:
		Expr_dec_args(expr->as.appl.left, cutoff);
		Expr_dec_args(expr->as.appl.right, cutoff);
		break;
	case LAMBDA_EXPR:
		expr->as.lambda.arg -= 1;
		Expr_dec_args(expr->as.lambda.body, cutoff);
		break;
	}
}

static void Expr_sub_free(Expr *expr, unsigned var)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		if (expr->as.var == var) {
			expr->type = BOUND_VAR_EXPR;
			expr->as.var = 1;
		}
		break;
	case BOUND_VAR_EXPR:
		break;
	case APPL_EXPR:
		Expr_sub_free(expr->as.appl.left, var);
		Expr_sub_free(expr->as.appl.right, var);
		break;
	case LAMBDA_EXPR:
		Expr_sub_free(expr->as.lambda.body, var);
		break;
	}
}

static void Expr_replace(Expr *dst, Expr *src)
{
	*dst = *src;
	free(src); // free only toplevel node
}

static void Expr_sub_bound(Expr *expr, unsigned var, Expr *val)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		break;
	case BOUND_VAR_EXPR:
		if (expr->as.var == var) {
			Expr *copy = Expr_copy(val);
			Expr_replace(expr, copy);
		}
		break;
	case APPL_EXPR:
		Expr *left_copy = Expr_copy(val);
		Expr_sub_bound(expr->as.appl.left, var, left_copy);
		Expr_drop(left_copy);
		Expr *right_copy = Expr_copy(val);
		Expr_sub_bound(expr->as.appl.right, var, right_copy);
		Expr_drop(right_copy);
		break;
	case LAMBDA_EXPR:
		Expr *copy = Expr_copy(val);
		Expr_inc_args(copy, expr->as.lambda.arg);
		Expr_sub_bound(expr->as.lambda.body, var, copy);
		Expr_drop(copy);
		break;
	}
}

void Expr_beta_reduce(Expr *redex)
{
	Expr *lambda = redex->as.appl.left, *arg = redex->as.appl.right;
	Expr_sub_bound(lambda, lambda->as.lambda.arg, arg);
	Expr_drop(arg);
	Expr_dec_args(lambda, lambda->as.lambda.arg);
	Expr_replace(lambda, lambda->as.lambda.body);
	Expr_replace(redex, lambda);
}

Expr *Lambda_new(Expr *arg, Expr *body)
{
	Expr *lambda = malloc(sizeof(*lambda));
	lambda->type = LAMBDA_EXPR;
	Expr_inc_args(body, 1);
	Expr_sub_free(body, arg->as.var);
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

Expr *Var_new(unsigned variable)
{
	Expr *var = malloc(sizeof(*var));
	var->type = FREE_VAR_EXPR;
	var->as.var = variable;
	return var;
}

static Expr *Bound_new(unsigned variable)
{
	Expr *var = malloc(sizeof(*var));
	var->type = BOUND_VAR_EXPR;
	var->as.var = variable;
	return var;
}

static Expr *BoundLambda_new(unsigned arg, Expr *body)
{
	Expr *lambda = malloc(sizeof(*lambda));
	lambda->type = LAMBDA_EXPR;
	lambda->as.lambda.arg = arg;
	lambda->as.lambda.body = body;
	return lambda;
}

Expr *Expr_copy(Expr *expr)
{
	switch (expr->type) {
	case FREE_VAR_EXPR:
		return Var_new(expr->as.var);
	case BOUND_VAR_EXPR:
		return Bound_new(expr->as.var);
	case APPL_EXPR:
		return Appl_new(
			Expr_copy(expr->as.appl.left),
			Expr_copy(expr->as.appl.right)
		);
	case LAMBDA_EXPR:
		return BoundLambda_new(
			expr->as.lambda.arg,
			Expr_copy(expr->as.lambda.body)
		);
	}
	return NULL;
}
