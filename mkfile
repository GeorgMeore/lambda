CC=gcc
CFLAGS=-g -Wall -Wextra
SRC=main.c expr.c input.c scanner.c parse.c reduce.c
OBJ=${SRC:%.c=%.o}
PROG=lambda

%.o: %.c
	$CC $CFLAGS -c $stem.c

$PROG: $OBJ
	cc -o $target $prereq

clean:
	rm -f $OBJ $PROG
