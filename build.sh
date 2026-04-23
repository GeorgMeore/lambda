#!/bin/sh -ex

CFLAGS="-g -Wall -Wextra -fsanitize=address,undefined"

cc $CFLAGS -o lambda main.c expr.c scanner.c parse.c node.c table.c arena.c
