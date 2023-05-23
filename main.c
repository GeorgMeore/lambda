#include <stdio.h>
#include <unistd.h>

#include "expr.h"
#include "input.h"
#include "scanner.h"
#include "parse.h"


int main()
{
	int tty = isatty(1);
	for (;;) {
		if (tty) {
			fprintf(stderr, "> ");
		}
		char *input = get_line();
		if (!input) {
			break;
		}
		Scanner scanner = input;
		Expr *expr = parse(&scanner);
		while (expr) {
			if (tty) {
				fprintf(stderr, "- ");
			}
			Expr_println(expr);
			Expr *reduced = Expr_beta_reduce(expr);
			Expr_drop(expr);
			expr = reduced;
		}
	}
	return 0;
}
