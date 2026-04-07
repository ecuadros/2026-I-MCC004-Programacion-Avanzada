#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "shape.h"

class Triangle : public Shape {
private:
    Distance base;
    Distance height;
public:
    Triangle(string name, Distance base, Distance height);
    Area GetArea() const override;
};

#endif // __TRIANGLE_H__