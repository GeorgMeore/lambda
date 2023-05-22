CC=gcc
CFLAGS=-Wall -Wextra
SRC=main.c expr.c
OBJ=${SRC:%.c=%.o}
PROG=lambda

%.o: %.c
	$CC $CFLAGS -c $prereq

$PROG: $OBJ
	cc -o $target $prereq

clean:
	rm -f $OBJ $PROG
