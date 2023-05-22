#include <stdio.h>

#include "expr.h"


int main()
{
	Expr *expr = Lambda_new(
		Var_new('c'),
		Appl_new(
			Lambda_new(Var_new('x'), Var_new('x')),
			Var_new('y')
		)
	);
	Expr_print(expr);
	putchar('\n');
	return 0;
}
