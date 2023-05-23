#include <stdio.h>

#include "expr.h"
#include "input.h"
#include "iter.h"
#include "parse.h"


int main()
{
	for (;;) {
		char *input = get_line();
		if (!input) {
			break;
		}
		CharIterator iter = input;
		Expr *expr = parse(&iter);
		if (!expr) {
			continue;
		}
		while (expr) {
			Expr_print(expr);
			Expr *reduced = Expr_beta_reduce(expr);
			Expr_drop(expr);
			expr = reduced;
		}
	}
	return 0;
}
