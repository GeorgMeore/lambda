#ifndef ITER_INCLUDED
#define ITER_INCLUDED

#include <stdio.h>


typedef struct {
	FILE *file;
	int  count;
	int  buf[2];
} Scanner;

Scanner Scanner_make(FILE *file);
int     Scanner_peek(Scanner *self);
int     Scanner_peek2(Scanner *self);
int     Scanner_next(Scanner *self);
void    Scanner_skip_to_eol(Scanner *self);

#endif // ITER_INCLUDED
