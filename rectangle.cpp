#include "rectangle.h"

Rectangle::Rectangle(string name, Distance width, Distance height)
    : Shape(name), width(width), height(height) {}

Area Rectangle::GetArea() const {
    return width * height;
}

