#include "scanner.h"


const char *Scanner_cursor(Scanner *self)
{
	return *self;
}

char Scanner_next(Scanner *self)
{
	char next = **self;
	*self += 1;
	return next;
}

char Scanner_peek(Scanner *self)
{
	return **self;
}

char Scanner_peek2(Scanner *self)
{
	return *(*self + 1);
}
