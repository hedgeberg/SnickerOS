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

	//rover logic -- seeks a correctly sized block
	free_list_entry_t * rover = heap_base_obj;
	while(!((rover->size >= size_pad) 
		  && (rover->alloc_flag == FREED))) {
		if(rover->next == heap_base_obj) return NULL;
		rover = rover->next;
	}
	//new block logic
	if(rover->size <= (size + 2*sizeof(free_list_entry_t) + MIN_BUFF_SIZE)){
		//too small to split in 2
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
	if(object_node->alloc_flag == FREED) return; 
	if(object_node == heap_base_obj){ //freeing top of heap
		if(object_node->next->alloc_flag == ALLOC){
			//next block is alocated
			object_node->alloc_flag = FREED;
		}
		else{
			//next block is freed
			object_node->size = object_node->size + 
				object_node->next->size + sizeof(free_list_entry_t);
			object_node->next = object_node->next->next;
			object_node->alloc_flag = FREED;
		}
	}
	else if(object_node->next == heap_base_obj){
		//freeing at end of heap
		object_node->prev->next = heap_base_obj;
		object_node->alloc_flag = FREED;
		if(object_node->prev->alloc_flag == FREED){
			//edge case for previous object freed
			object_node->prev->size += 
				(object_node->size + sizeof(free_list_entry_t));
		}
	}
	else if((object_node->prev->alloc_flag == ALLOC) 
			&& (object_node->next->alloc_flag == ALLOC)){
		//object is between 2 allocated blocks
		object_node->alloc_flag = FREED;
	}
	else if((object_node->prev->alloc_flag == FREED)
			&& (object_node->next->alloc_flag == ALLOC)){
	//block below is allocated, block above is free
		//newly freed block is merged into block above
		object_node->prev->size += 
					(object_node->size + sizeof(free_list_entry_t));
		object_node->prev->next = object_node->next;
		object_node->next->prev = object_node->prev;
		object_node->alloc_flag = FREED;
	}
	else if((object_node->prev->alloc_flag == ALLOC) && 
			(object_node->next->alloc_flag == FREED)){
	//block below is free, block above is allocated
		//block below is merged into newly freed block
		object_node->size += 
				object_node->next->size + sizeof(free_list_entry_t);
		object_node->next->next->prev = object_node;
		object_node->next = object_node->next->next;
		object_node->alloc_flag = FREED;

	}
	else{
	//block is between 2 free blocks
		//block below and newly freed block is merged into block above
		object_node->prev->size += (object_node->size + 
				object_node->next->size + 2*sizeof(free_list_entry_t));
		object_node->next->next->prev = object_node->prev;
		object_node->prev->next = object_node->next->next;
		object_node->alloc_flag=FREED;
	}
	return;
}

//int check_heap_integrity();
