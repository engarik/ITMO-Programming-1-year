/* Kornilov Nikita, M3102, 7.10.2020, lab 11, var. 8 */

#include <stdio.h>
#include "regularTriangle.h"

int main( void ) {

    RegularTriangle triangle;

    printf("Input (.)A coordinates:  ");
    scanf("%f %f", &triangle.x1, &triangle.y1);
    printf("Input (.)B coordinates:  ");
    scanf("%f %f", &triangle.x2, &triangle.y2);
    printf("Input (.)C coordinates:  ");
    scanf("%f %f", &triangle.x3, &triangle.y3);
    
    printf("Perimeter: %f\n", regularTrianglePerimeter(triangle));
    printf("Area: %f\n", regularTriangleArea(triangle));
    
    return 0;
}
