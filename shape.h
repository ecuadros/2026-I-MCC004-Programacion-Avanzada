#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <string>
#include "types.h"
using namespace std;

class Shape {
private:
    string name;
public:
    Shape(string name);
    virtual ~Shape() = default;
    virtual Area GetArea() const = 0; // Metodo virtual puro
    virtual string GetName() const;
};

#endif // __SHAPE_H__