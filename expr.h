#ifndef EXPR_INCLUDED
#define EXPR_INCLUDED

typedef enum {
	LAMBDA_EXPR,
	APPL_EXPR,
	FREE_VAR_EXPR,
	BOUND_VAR_EXPR,
} ExprType;

typedef struct Expr Expr;

typedef unsigned VarExprValue;

typedef struct {
	int arg;
	Expr *body;
} LambdaExprValue;

typedef struct {
	Expr *left;
	Expr *right;
} ApplExprValue;

// NOTE: To solve the problem of bound variable name collisions,
// this implementation uses automatic "renaming".
// When a term is analyzed, bound variables are replaced with positive integers
// that correspond to the nesting level of the binding lambda.
// During a reduction step these values are appropriately recalculated.
// (This is kind of like De Bruijn indices, but different)
// Here are some examples:
//   λx.xa               -> λ1.1a
//   λx.λy.xy            -> λ1.λ2.12
//   λx.(λf.fx)(λg.gx)fg -> λ1.(λ2.21)(λ2.21)fg
typedef union {
	VarExprValue var;
	LambdaExprValue lambda;
	ApplExprValue appl;
} ExprValue;

struct Expr {
	ExprType type;
	ExprValue as;
};

void  Expr_print(Expr *expr);
void  Expr_println(Expr *expr);
Expr* Lambda_new(Expr *arg, Expr *body);
Expr* Appl_new(Expr *left, Expr *right);
Expr* Var_new(unsigned variable);
Expr* Expr_copy(Expr *expr);
void  Expr_drop(Expr *expr);
void  Expr_beta_reduce(Expr *redex);

#endif // EXPR_INCLUDED
