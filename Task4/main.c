/* Kornilov Nikita, M3102, 11.09.2020, lab 4, var. 8 */
#include <stdio.h>

void main( void ) {
	int x1, x2;
	
	printf("Please input number in range (-56 ; 56): ");
	scanf("%i", &x1);
	
	printf("In limited range? %s \n", (x1 > -56) && (x1 < 56) ? "Yes." : "No.");
	
	printf("Please input number: ");
	scanf("%i", &x2);
	
	printf("Bit on 8th position is %i \n", (x2>>7) & 1);
}
