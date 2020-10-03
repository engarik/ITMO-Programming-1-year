/* Kornilov Nikita, M3102, 14.09.2020, lab 5, var. 8 */

#include <stdio.h>

int main(void) {
	const int arraySize = 8;
	const int n = 2;
	
	int i, j, k;
	int array[] = {77, 12, 74, 34, 56, 78, 234, 678};
	
	int matrix1[2][2] = {{2, 1}, {1, 3}};
	int matrix2[2][2] = {{1, 2}, {3, 1}};
	int matrixResult[2][2];
	
	for (i = 0; i < arraySize; i++)
		printf("%i ", array[i]);
	printf("\n");
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			matrixResult[i][j] = 0;
	        for(k = 0; k < n; k++)
	            matrixResult[i][j] += matrix1[i][k] * matrix2[k][j];
    	}
	}
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf("%i ", matrixResult[i][j]);
		printf("\n");
	}
		
	return 0;
}
