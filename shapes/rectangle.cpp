#include "rectangle.h"

Rectangle::Rectangle(string name, ostream &os, Distance width, Distance height)
    : Shape(name, os), m_width(width), m_height(height) {
        m_os << "Construyendo Rectangle: " << GetName() << endl;
    }

Rectangle::~Rectangle() {
    m_os << "Destruyendo  Rectangle: " << GetName() << endl;
}

Area Rectangle::GetArea() const {
    return m_width * m_height;
}

