#include "table.h"

#include <stddef.h>

#include "expr.h"


#define INDEX(x) ((x - 'A')%26)

int Table_has(Table table, unsigned key)
{
	return table[INDEX(key)] != NULL;
}

Expr *Table_lookup(Table table, unsigned key)
{
	return table[INDEX(key)];
}

void Table_set(Table table, unsigned key, Expr *value)
{
	table[INDEX(key)] = value;
}

void Table_clear(Table table)
{
	for (int i = 0; i < 26; i++) {
		if (table[i]) {
			Expr_drop(table[i]);
		}
	}
}
