
#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "rectangle.h"

class Square : public Rectangle {
public:
    Square(string name, ostream &os, Distance side) : Rectangle(name, os, side, side) {
        m_os << "Construyendo Square: " << GetName() << endl;
    }
    ~Square() {
        m_os << "Destruyendo  Square: " << GetName() << endl;
    }
};

#endif // __SQUARE_H__