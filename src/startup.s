startup:
	MRC     p15, 0, r0, c1, c0, 0
	BIC     r0, r0, #0x1                 
	MCR     p15, 0, r0, c1, c0, 0         
	LDR sp, =stack_top
	BL setup_vectors
	BL ps7_init
	LDR sp, =stack_main
	BL main
	B .
