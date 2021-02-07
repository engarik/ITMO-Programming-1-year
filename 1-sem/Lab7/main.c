/* Kornilov Nikita, M3102, 5.10.2020, lab 7, var. 8 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum LampType {
	incandescent,
	halogen,
	led,
	fluorescent,
	neon
};

union CardReader {
	struct BitField {
		unsigned power : 1;
		unsigned sdCard : 1;
		unsigned compactFlash : 1;
		unsigned memoryStick : 1;
	};
};

struct Triangle {
	int x0, x1, x2, y0, y1, y2;
	double p, s;
};

int main(void) {
	printf("Your halogen lamp is %i\n", halogen);
	
	struct Triangle triangle;
	triangle.x0 = 1, triangle.y0 = 3;
	triangle.x1 = -2, triangle.y1 = 5;
	triangle.x2 = -5, triangle.y2 = -1;
	triangle.s = fabs(0.5 * ((triangle.x1 - triangle.x0) * (triangle.y2 - triangle.y0) - (triangle.x2 - triangle.x0) * (triangle.y1 - triangle.y0)));
	triangle.p = sqrt((triangle.x0 - triangle.x1) * (triangle.x0 - triangle.x1) + (triangle.y0 - triangle.y1) * (triangle.y0 - triangle.y1)) + sqrt((triangle.x0 - triangle.x2) * (triangle.x0 - triangle.x2) + (triangle.y0 - triangle.y2) * (triangle.y0 - triangle.y2)) + sqrt((triangle.x1 - triangle.x2) * (triangle.x1 - triangle.x2) + (triangle.y1 - triangle.y2) * (triangle.y1 - triangle.y2));
	printf("Triangle A(%i; %i) B(%i; %i) C(%i; %i)\n", triangle.x0, triangle.y0, triangle.x1, triangle.y1, triangle.x2, triangle.y2);
	printf("Area: %f\n", triangle.s);
	printf("Perimeter: %f\n", triangle.p);
	
	union CardReader card;
    printf("Input hex number: ");
    scanf("%x", &(card));
    printf("CardReader:\n");
    printf("Power: %s\n", (card.power) ? "On" : "Off");
    printf("SD-card: %s\n", (card.sdCard) ? "On" : "Off");
    printf("Compact flash: %s\n", (card.compactFlash) ? "On" : "Off");
	printf("Memory stick: %s\n", (card.memoryStick) ? "On" : "Off");
	
	return 0;
}
