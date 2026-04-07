#include <cmath>
#include "circle.h"

Circle::Circle(string name, Distance radius)
    : Shape(name), radius(radius) {}

Area Circle::GetArea() const {
    return M_PI * radius * radius;
}