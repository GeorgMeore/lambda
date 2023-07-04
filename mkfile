CC=gcc
CFLAGS=-g -Wall -Wextra
SRC=main.c expr.c scanner.c parse.c node.c table.c arena.c
OBJ=${SRC:%.c=%.o}
PROG=lambda

%.o: %.c
	$CC $CFLAGS -c $stem.c

$PROG: $OBJ
	cc -o $target $prereq

# generate dependency list
<|$CC -MM $SRC

clean:V:
	rm -f $OBJ $PROG
