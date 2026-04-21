#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "shape.h"

class Circle : public Shape {
private:
    Distance m_radius;
public:
    Circle(string name, ostream &os, Distance radius);
    virtual ~Circle();
    Area GetArea() const override;
};

#endif // __CIRCLE_H__