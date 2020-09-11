/* Kornilov Nikita, M3102, 10.09.2020, var. 8*/
#include <stdio.h>

void main( void ) {
	int integer = 0, input;
	char character = 0;
	
	printf("What would you like to input? 1 - integer, 2 - character?\n");
	scanf("%i", &input);
	getchar();
	
	if (input == 1)
	{
		printf("Input your integer: ");
		scanf("%i", &integer);
		printf("Your input integer was: %i\n", integer);
	}
	else
	{
		printf("Input your character: ");
		scanf("%c", &character);
		printf("Your input character was: %c\n", character);
	}
}

