#include "scanner.h"

#include <stdio.h>


Scanner Scanner_make(FILE *file)
{
	return (Scanner){.file = file, .count = 0};
}

static void Scanner_getc(Scanner *self)
{
	int c = fgetc(self->file);
	if (c == EOF || c == '\n') {
		self->buf[self->count] = '\0';
	} else {
		self->buf[self->count] = c;
	}
	self->count += 1;
}

int Scanner_next(Scanner *self)
{
	if (self->count == 0) {
		Scanner_getc(self);
	}
	int c = self->buf[0];
	self->buf[0] = self->buf[1];
	self->count -= 1;
	return c;
}

void Scanner_skip_to_eol(Scanner *self)
{
	while (Scanner_peek(self) != '\0') {
		Scanner_next(self);
	}
	Scanner_next(self);
}

int Scanner_peek(Scanner *self)
{
	while (self->count < 1) {
		Scanner_getc(self);
	}
	return self->buf[0];
}

int Scanner_peek2(Scanner *self)
{
	while (self->count < 2) {
		Scanner_getc(self);
	}
	return self->buf[1];
}
