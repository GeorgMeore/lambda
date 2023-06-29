#ifndef PARSE_INCLUDED
#define PARSE_INCLUDED

#include "scanner.h"
#include "node.h"
#include "arena.h"

Node *parse(Arena *a, Scanner *scanner);

#endif // PARSE_INCLUDED
