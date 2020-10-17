#include "regularTriangle.h"

float getLength(float x1, float x2, float y1, float y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float regularTrianglePerimeter(struct RegularTriangle triangle) {
	return 
	getLength(triangle.x1, triangle.x2, triangle.y1, triangle.y2) +
	getLength(triangle.x1, triangle.x3, triangle.y1, triangle.y3) +
	getLength(triangle.x2, triangle.x3, triangle.y2, triangle.y3);
}

float regularTriangleArea(struct RegularTriangle triangle) {
    return 0.5f * getLength(triangle.x1, triangle.x2, triangle.y1, triangle.y2)
    * getLength(triangle.x2, triangle.x3, triangle.y2, triangle.y3);
}


