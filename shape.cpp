#include "shape.h"

Shape::Shape(string name) : name(name) {}

string Shape::GetName() const {
    return name;
}

