#ifndef H_REGULARTRIANGLE
#define H_REGULARTRIANGLE
#include "math.h"

struct RegularTriangle{
    float x1, y1;
    float x2, y2;
    float x3, y3;
};

float regularTrianglePerimeter(struct RegularTriangle triangle);
float regularTriangleArea(struct RegularTriangle triangle);
#endif
