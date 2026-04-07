#include "triangle.h"

Triangle::Triangle(string name, Distance base, Distance height)
    : Shape(name), base(base), height(height) {}

Area Triangle::GetArea() const {
    return 0.5 * base * height;
}