#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "shape.h"

class Rectangle : public Shape {
private:
    Distance width;
    Distance height;
public:
    Rectangle(string name, Distance width, Distance height);
    Area GetArea() const override;
};

#endif // __RECTANGLE_H__