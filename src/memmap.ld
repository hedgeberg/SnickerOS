SECTIONS
{
     . = 0x10000;
     start = 0x10000; /* Informs .elf uploaders on  start address*/
     .ro : {
	  startup.o (.text)
	  *(.text)
	  *(.rodata)
	 }
     .rw : {
	  *(.data)
	  *(.bss)	  *(COMMON)
	 }
     . = ALIGN(8);
	/* 
     heap_low = .; 
     . = . + 0x10000; 
     heap_top = .; 
     . = . + 0x10000; 
     */
     . = . + 0x10000;
     stack_top = .; /* for startup.s */
     stack_main = 0x3FFFFFF0;
}

