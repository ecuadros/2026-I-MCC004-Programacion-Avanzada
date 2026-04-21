#include "triangle.h"

Triangle::Triangle(string name, ostream &os, Distance base, Distance height)
    : Shape(name, os), m_base(base), m_height(height) {
        m_os << "Construyendo Triangle: " << GetName() << endl;
    }

Triangle::~Triangle() {
    m_os << "Destruyendo  Triangle: " << GetName() << endl;
}

Area Triangle::GetArea() const {
    return 0.5 * m_base * m_height;
}