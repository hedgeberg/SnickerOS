#include "io_print.h"
#include "stdarg.h"

#define UART0_TxRxFIFO0 ((unsigned int *) (UART0_BASE + 0x30))
#define UART0_BASE 0xe0000000

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

int sprintf(char * buffer, char * format_str, ...)
{

	//int count = 0;

	//count formatter entries
	/*
	for(int i = 0; format_str[i] != '\0'; i++){
		if(format_str[i] == '%') count += 1;
	}
	*/
	//start parsing varargs
	va_list ap;
	va_start(ap, format_str);

	int i = 0;
	int j = 0;
	while(format_str[i] != '\0'){
		if(format_str[i] != '%'){
			buffer[j] = format_str[i];
			j++;
			i++;
		} else {
			switch(format_str[i+1]){

				case 'd' :
					buffer[j] = (char)('0' + va_arg(ap, int));
					j += 1;
					i += 2;
					break;

				case 's' :
					//include str
					break;

				case 'x' :
					//include hex
					break;

				default : 
					//all else, throw an error
					break;

			}
		}
	}
	buffer[j] = '\0';
	return j;
}