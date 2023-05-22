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
		Expr_print(expr);
		putchar('\n');
		Expr_drop(expr);
	}
	return 0;
}
