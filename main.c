#include <stdio.h>
#include <unistd.h>

#include "expr.h"
#include "input.h"
#include "scanner.h"
#include "parse.h"
#include "reduce.h"


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
	return 0;
}
