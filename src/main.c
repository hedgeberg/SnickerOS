#include  "lib/memory.h"
#include  "lib/ps7_init_gpl.h"
#include <stdint.h>
#include <stddef.h>
#include  "lib/heap.h"

#define HEAP_BASE 0x00800000
#define UART0_BASE 0xe0000000
#define UART0_TxRxFIFO0 ((unsigned int *) (UART0_BASE + 0x30))

volatile unsigned int * const TxRxUART0 = UART0_TxRxFIFO0;
	 



int puts(const char *s) 
{
  while(*s != '\0') 
  {     /* Loop until end of string */
    *TxRxUART0 = (unsigned int)(*s); /* Transmit char */
    s++; /* Next char */
  }
    return 0;
}

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

  char * blah_pointer = malloc(5);
  char * another_one = malloc(4);
  blah_pointer[0] = 'a';
  another_one[2] = 'd';
  free(blah_pointer);
  char * one_mo_again = malloc(4);
  one_mo_again[0] = 'A';
  puts("Hello world!\r\n");

  while(1);
}


