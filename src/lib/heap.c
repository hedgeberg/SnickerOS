#include <stdint.h>
#include "heap.h"

#define HEAP_MAX_SIZE 0x100000 //1 MibbiByte
#define MIN_BUFF_SIZE 4
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

	//padding logic (round up to 4-byte sizes)
	size_t size_pad = (size + 3) & ~3; //thx scanlime!
	/*
	if(size & 0x00000003){ //0b0..0011
		size_pad = size & 0xFFFFFFFC; //0b1...1100
		size_pad += 4;
	} else {
		size_pad = size;
	}*/
	//rover logic -- seeks a correctly sized block
	free_list_entry_t * rover = heap_base_obj;
	while(!((rover->size >= size_pad) 
		  && (rover->alloc_flag == FREED))) {
		if(rover->next == heap_base_obj) return NULL;
		rover = rover->next;
	}
	//new block logic
	if(rover->size <= (size + 2*sizeof(free_list_entry_t) + MIN_BUFF_SIZE)){
		//don't make new node
		rover->alloc_flag = ALLOC;
	} else {
		free_list_entry_t * new_entry = 
			(free_list_entry_t *)((uint8_t *)(rover + 1) + size_pad);
		new_entry->prev = rover;
		new_entry->next = rover->next;
		rover->next = new_entry;
		new_entry->size = rover->size - size_pad - sizeof(free_list_entry_t);
		new_entry->alloc_flag = FREED;
		rover->size = size_pad;
		rover->alloc_flag = ALLOC;
	}
	//zeroing logic
	char * buffer = (char *)(void *)(rover + 1);
	for(int i = 0; i < size_pad; i++) buffer[i] = (char)0;
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