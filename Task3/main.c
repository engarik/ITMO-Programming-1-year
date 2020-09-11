/* Kornilov Nikita, M3102, 11.09.2020, lab 3, var. 8*/
#include <stdio.h>

void main( void ) {
	int x1, x2;

	printf("Please input decimal number: ");
	scanf("%i", &x1);

	printf("Your number in octal is: %o \n", x1);

	printf("Your number in hex is: %X \n", x1);

	printf("Your number in hex >> 4: %X \n", x1 >> 4);

	printf("Your number in hex is: %X \n", x1);

	printf("Your number in hex but inverted: %X \n", ~x1);

	printf("Please input new hexadecimal number: ");
	scanf("%X", &x2);

	printf("x1 ^ x2 in hex is: %X \n", x1 ^ x2);

}
