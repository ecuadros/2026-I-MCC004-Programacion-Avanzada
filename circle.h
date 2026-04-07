#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "shape.h"

class Circle : public Shape {
private:
    Distance radius;
public:
    Circle(string name, Distance radius);
    Area GetArea() const override;
};

#endif // __CIRCLE_H__