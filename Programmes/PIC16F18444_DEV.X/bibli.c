#include <xc.h>
#include "conbits.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>

// Bibliothèque de fonction utiles / necessaires aux applications
//////////////////////////////////////////////////////////////////////////////////////////////Hex_to_BCD
int Hex_to_BCD(int hex_val)
{
    int bcd_val;
    bcd_val = (hex_val / 10) << 4;
    bcd_val = bcd_val | (hex_val % 10);
    return (bcd_val);
}
//////////////////////////////////////////////////////////////////////////////////////////////BCD_to_Hex
uint8_t BCD_to_Hex(uint8_t bcd_val)
{
    uint8_t hex_val;
    hex_val = (((bcd_val & 0xF0) >> 4)* 10) + (bcd_val & 0x0F);
    return hex_val;
}
////////////////////////////////////////////////////////////////////////////////////////////// ATOI
// inline function to swap two numbers
inline void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int form)
{
	// invalid input
	if (form < 2 || form > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	int i = 0;
	while (n)
	{
		int r = n % form;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / form;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If form is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other form, value is always considered unsigned
//	if (value < 0 && form == 10)
//		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}
