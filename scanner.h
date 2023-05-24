#ifndef ITER_INCLUDED
#define ITER_INCLUDED

typedef const char *Scanner;

const char* Scanner_cursor(Scanner *self);
char        Scanner_peek(Scanner *self);
char        Scanner_peeek(Scanner *self);
char        Scanner_next(Scanner *self);

#endif // ITER_INCLUDED
