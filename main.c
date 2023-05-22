#include <stdio.h>

#include "expr.h"
#include "input.h"


int main()
{
	for (;;) {
		char *input = get_line();
		if (!input) {
			break;
		}
		printf("Got input: %s\n", input);
		Expr *expr = Lambda_new(
			Var_new('c'),
			Appl_new(
				Lambda_new(Var_new('x'), Var_new('x')),
				Var_new('y')
			)
		);
		Expr_print(expr);
		Expr_drop(expr);
		putchar('\n');
	}
	return 0;
}
