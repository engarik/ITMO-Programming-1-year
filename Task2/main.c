/* Kornilov Nikita, M3102, 10.09.2020, lab 2, var. 8*/
#include <stdio.h>
#include <math.h>

void main( void ) {
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	double z0, z1;

	printf("x0: ");
	scanf("%i", &x0);
	printf("y0: ");
	scanf("%i", &y0);
	printf("x1: ");
	scanf("%i", &x1);
	printf("y1: ");
	scanf("%i", &y1);

	z0 = pow(cos(x0), 4.0) + pow(sin(y0), 2.0) + pow(sin(2 * x0), 2.0) / 4 - 1;
	z1 = sin(y1 + x1) * sin(y1 - x1);
	printf("z0: %f \nz1: %f", z0, z1);

}
