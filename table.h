#ifndef ENV_INCLUDED
#define ENV_INCLUDED

#include "expr.h"

typedef Expr *Table[26];

int   Table_has(Table table, unsigned key);
Expr* Table_lookup(Table table, unsigned key);
void  Table_set(Table table, unsigned key, Expr *value);
void  Table_clear(Table table);

#endif // ENV_INCLUDED
