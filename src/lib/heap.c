#include <stdint.h>
#include "heap.h"

#define HEAP_MAX_SIZE 0x100000 //1 MibbiByte
#define ALLOC 1
#define FREED 0

typedef struct free_list_entry{
	struct free_list_entry * prev;
	struct free_list_entry * next;
	uint8_t alloc_flag;
	size_t size;
}free_list_entry_t;


static free_list_entry_t * heap_base_obj;


void initialize_heap(void * heap_base_addr){
	free_list_entry_t * heap_base = (free_list_entry_t *)heap_base_addr;
	heap_base->prev = heap_base;
	heap_base->next = heap_base;
	heap_base->alloc_flag = FREED;
	heap_base->size = HEAP_MAX_SIZE - sizeof(free_list_entry_t);

	heap_base_obj = heap_base;
	return;
}

void * malloc(size_t size){
/*
	cases:
		1. heap empty. heap_prev == heap_next
		2. heap full. heap_next + size_desired > HEAP_BASE + HEAP_MAX_SIZE
		3. heap roaming
*/
	free_list_entry_t * rover = heap_base_obj;
	while(!((rover->size >= size) 
		  && (rover->alloc_flag == FREED))) {
		rover = rover->next;
		//return NULL if we hit end of heap
	}
	//either size == rover->size (or isnt large enough to 
		//create an unorphaned header)
		//just switch freed to alloced
	//else{
		free_list_entry_t * new_entry = 
			(free_list_entry_t *)((uint8_t *)(rover + 1) + size);
		new_entry->prev = rover;
		new_entry->next = rover->next;
		rover->next = new_entry;
		new_entry->size = rover->size - size - sizeof(free_list_entry_t);
		new_entry->alloc_flag = FREED;
		rover->size = size;
		rover->alloc_flag = ALLOC;
	//}
	return (void *)(rover + 1);
}

void free(void * object){
	free_list_entry_t * object_node = 
		((free_list_entry_t *)object) - 1;
	if(object_node == heap_base_obj){ //freeing top of heap
		if(object_node->next->alloc_flag == ALLOC){
			object_node->alloc_flag = FREED;
		}
		else{
			object_node->size = object_node->size + 
				object_node->next->size + sizeof(free_list_entry_t);
			object_node->next = object_node->next->next;
			object_node->alloc_flag = FREED;
		}
	}
	//edge case for freeing at the end of the heap
	//object is between 2 allocated blocks
		//set alloc_flag = FREED
	//block below is allocated, block above is free
		//newly freed block is merged into block above
	//block below is free, block above is allocated
		//block below is merged into newly freed block
	//block is between 2 free blocks
		//block below and newly freed block is merged into block above
	return;
}

//int check_heap_integrity();