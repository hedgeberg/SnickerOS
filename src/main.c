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

  //change sp to be 0x3FFF_FFF0
  //set heap_base to be 0x0080_0000

  initialize_heap((void *)HEAP_BASE);

  char * strbuf = malloc(20);
  sprintf(strbuf, "test %u 0x%x %s\r\n", 7, 0xADADCAFE, "this is a string");
  puts(strbuf);
  puts("Hello world!\r\n");

  while(1);
}


