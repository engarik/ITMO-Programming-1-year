/* Kornilov Nikita, M3102, 7.10.2020, lab 10, var. 8 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfDigits(int n) {
	int digits = 0;
	
	if (n == 0) {
		return 1;
	}
	
	while (n > 0) {
		digits++;
		n /= 10;
	}
	return digits;
}

int *numberToArray(int n, int digits) {
	int i, tmp;
	int *array = (int*)malloc(digits * sizeof(int));

	for (i = 0; i < digits; i++) {
		array[i] = n % 10;
		n /= 10;
	}
	
	for (i = 0; i < digits / 2; i++) {
		tmp = array[i];
		array[i] = array[digits - i - 1];
		array[digits - i - 1] = tmp;
	}
	
	return array;
}

void printArray(int *array, int arrayLength) {
	int i;
	
	for (i = 0; i < arrayLength; i++) {
		printf("%i ", array[i]);
	}
	
	printf("\n");
}

char *removeSpaces(char *str, int pos, int strLength) {
	int i, n = 0, flag = 1;
	char *resStr;
	
	if (pos >= strLength)
		return str;
		
	if (
		str[pos] == ' ' || str[pos] == '(' 	|| 
		str[pos] == '{' || str[pos] == '\'' || 
		str[pos] == '"' || str[pos] == '<' 	||
		str[pos] == '[' || str[pos] == '|' 	
	) {
		i = pos + 1;
		while (i < strLength && str[i] == ' ') {
			i++;
			n++;
		}
		
		resStr = (char*)malloc((strLength - n) * sizeof(char));
		
		for (i = 0; i < pos + 1; i++) {
			resStr[i] = str[i];
		}
		
		for (i = pos + 1 + n; i < strLength; i++) {
			resStr[i - n] = str[i];
		}
	
		return removeSpaces(resStr, pos + 1, strLength - n);
	}
	else if (str[pos] == '.') {
		i = pos + 1;
		while (i < strLength) {
			if (str[i] == ' ') {
				i++;
				n++;
			} else {
				return removeSpaces(str, pos + 1, strLength);
			}
		}
		//printf("removed %i spaces after last dot\n", n);
		resStr = (char*)malloc((strLength - n) * sizeof(char));
	
		for (i = 0; i < pos + 1; i++) {
			resStr[i] = str[i];
		}
		
		return removeSpaces(resStr, pos + 1, strLength - n);
	}
	
	return removeSpaces(str, pos + 1, strLength);	
}

int main(void) {
	int strLength, n, digits;
	int *digitsArray;
	char *str, *result;
	
	printf("Please input n: ");
	scanf("%i", &n);
	
	digits = numberOfDigits(n);
	digitsArray = numberToArray(n, digits);
	printArray(digitsArray, digits);
	
	
	printf("Please input string length: ");
	scanf("%i", &strLength);
	
	str = (char*)malloc(strLength * sizeof(char));
	
	printf("Please input string: ");
	getchar();
	gets(str);
	
	result = removeSpaces(str, 0, strLength);
	
	printf("%s", result);
	
	free(str);
	free(result);

	return 0;
}


