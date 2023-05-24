#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "scanner.h"
#include "parse.h"
#include "node.h"
//#include "expr.h"
//#include "reduce.h"


void execute(Node *ast)
{
	if (ast->type == DEF_NODE) {
		printf("definition: ");
		Node_println(ast);
	} else if (ast->type == APPL_NODE) {
		printf("evaluation: ");
		Node_println(ast);
		//for (;;) {
		//	if (tty) {
		//		fprintf(stderr, "- ");
		//	}
		//	Expr_println(expr);
		//	if (!reduce(expr)) {
		//		break;
		//	}
		//}
		//Expr_drop(expr);
	}
}

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
		Node *ast = parse(&scanner);
		if (!ast) {
			continue;
		}
		execute(ast);
		Node_drop(ast);
	}
	return 0;
}
