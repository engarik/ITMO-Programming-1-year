/* Kornilov Nikita, M3102, 10.09.2020, var. 8*/
#include <stdio.h>

void main( void ) {
	int integer = 0;
	char character = 0;
	char input[1];
	
	printf("What would you like to input? 1 - integer, 2 - character?\n");
	gets(input);
	
	while (input[0] != '1' && input[0] != '2')
	{
		printf("I don't understand this command, try again please...\n");
		gets(input);
	}
	
	if (input[0] == '1')
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

