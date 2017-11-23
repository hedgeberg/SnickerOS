#include "lib/memory.h"
#include "lib/ps7_init_gpl.h"
#include <stdint.h>
#include <stddef.h>
#include "lib/heap.h"
#include "lib/io_print.h"

#define HEAP_BASE 0x00800000


void setup_vectors(){
  uint32_t vector = 0xEAFFFFFE;
  uint32_t i;
  uint32_t base = 0x4;
  uint32_t address;
  for(i = 0; i < 7; i++){
    address = base + (i*0x4);
    *(uint32_t *)(address) = vector;
  }
  return;
}

void main() 
{
  initialize_heap((void *)HEAP_BASE);

  int status; 

  void * ptr1 = malloc(20*sizeof(char));
  status = check_heap_integrity();
  printf("status after alloc 1 = %u\r\n", status);
  
  void * ptr2 = malloc(20*sizeof(char));
  status = check_heap_integrity();
  printf("status after alloc 2 = %u\r\n", status);

  free(ptr1);
  status = check_heap_integrity();
  printf("status after free 1 = %u\r\n", status);

  free(ptr2);
  status = check_heap_integrity();
  printf("status after free 2 = %u\r\n", status);

  void * ptr3 = malloc(600*sizeof(char));
  status = check_heap_integrity();
  printf("status after alloc 3 = %u\r\n", status);

  free(ptr3);
  status = check_heap_integrity();
  printf("status after free 3 = %u\r\n", status);

  //printf("test %u 0x%x %s\r\n", 7, 0xADADCAFE, "this is a string");
  //puts("Hello world!\r\n");

  while(1);
}


