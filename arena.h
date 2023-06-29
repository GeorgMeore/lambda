#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED

#include <stddef.h>


typedef struct Page Page;

struct Page {
	Page   *next;
	char   *data;
	size_t taken;
};

Page *Page_new(size_t page_size);
void  Page_drop(Page *self);

typedef struct Arena {
	Page   *first;
	size_t page_size;
} Arena;

Arena *Arena_new(size_t page_size);
void  *Arena_alloc(Arena *self, size_t bytes);
void   Arena_reset(Arena *self);
void   Arena_drop(Arena *self);


#endif // ARENA_INCLUDED
