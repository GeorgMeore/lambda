#ifndef PARSE_INCLUDED
#define PARSE_INCLUDED

#include "scanner.h"
#include "expr.h"

Expr *parse(Scanner *scanner);

#endif // PARSE_INCLUDED
