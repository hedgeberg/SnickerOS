#ifndef IS_HEAP_INCLUDED
#define IS_HEAP_INCLUDED 1

#include <stddef.h>

void initialize_heap(void * heap_base_addr);

void * malloc(size_t size);

void free(void * obj);

#endif