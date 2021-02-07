/* Kornilov Nikita, M3102, 3.10.2020, lab 6, var. 8 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	const int n = 4;
    int arrayA[n];
    int i, j;

    for (i = 0; i < n; i++) {
        scanf("%i", &arrayA[i]);
	}

    printf("Your array: ");
    for(i = 0; i < n; i++) {
        printf("%i ", *(arrayA + i));
	}
    printf("\n");

    int *arrayB;
    arrayB = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        scanf("%i", &arrayB[i]);
	}

    printf("Your array: ");
    for(i = 0; i < n; i++) {
        printf("%i ", *(arrayB + i));
	}
	printf("\n");
	
    free(arrayB);
    return 0;
}
