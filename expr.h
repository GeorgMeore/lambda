#ifndef EXPR_INCLUDED
#define EXPR_INCLUDED

typedef enum {
	LAMBDA_EXPR,
	APPL_EXPR,
	FREE_VAR_EXPR,
	BOUND_VAR_EXPR,
} ExprType;

typedef struct Expr Expr;

typedef struct {
	int arg;
	Expr *body;
} LambdaValue;

typedef struct {
	Expr *left;
	Expr *right;
} ApplValue;

typedef union {
	int var;
	LambdaValue lambda;
	ApplValue appl;
} ExprValue;

struct Expr {
	ExprType type;
	ExprValue as;
};

void Expr_print(Expr *expr);
void Expr_println(Expr *expr);
Expr *Lambda_new(Expr *arg, Expr *body);
Expr *Appl_new(Expr *left, Expr *right);
Expr *Var_new(int variable);
Expr *Expr_copy(Expr *expr);
void Expr_drop(Expr *expr);
Expr *Expr_beta_reduce(Expr *expr);

#endif // EXPR_INCLUDED
