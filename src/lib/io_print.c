#include "io_print.h"
#include "stdarg.h"
#include "stdint.h"

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

	//LIST ASSUMPTIONS:
	//u: assumes unsigned integer
	//s: null-terminated string
	//x: assumes 32-bit number
	//start parsing varargs
	va_list ap;
	va_start(ap, format_str);

	int i = 0;
	int j = 0;
	int varwidth = 0;
	int varint;
	char * passed_buf;

	char intbuf[11];
	while(format_str[i] != '\0'){
		varint = 0;
		varwidth = 0;
		if(format_str[i] != '%'){
			buffer[j] = format_str[i];
			j++;
			i++;
		} else {
			switch(format_str[i+1]){

				case 'u' :
					varint = va_arg(ap, int);
					if(varint == 0){
						buffer[j] = '0';
						j += 1;
						i += 2;
						break;
					}
					while(varint > 0){
						intbuf[varwidth] = (char)('0' + (varint % 10));
						varint = varint/10;
						varwidth += 1;
					}
					for(int k = varwidth; k > 0; k--){
						buffer[j + (varwidth - k)] = intbuf[k - 1];
					}
					j += varwidth;
					i += 2;
					break;

				case 's' :
					passed_buf = va_arg(ap, char *);
					for(int k = 0; passed_buf[k] != '\0'; k++){
						buffer[j] = passed_buf[k];
						j++;
					}
					i += 2;
					break;

				case 'x' :
					varint = va_arg(ap, uint32_t);
					for(int k = 7; k >= 0; k--){
						buffer[j + (7-k)] = "0123456789ABCDEF"[
										(varint >> (k * 4)) & 0x0F];
					}
					j += 8;
					i += 2;
					break;

				default : 
					return 0;
					break;

			}
		}
	}
	buffer[j] = '\0';
	return j;
}