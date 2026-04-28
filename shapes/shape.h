#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <string>
#include <iostream>
#include <sstream> // ostringstream
#include "../types.h"
using namespace std;

/**
 * @brief Shape generico
 * 
 */
class Shape {
private:
    string m_name; // nombre de la figura
protected:
    ostream &m_os; // ostream para imprimir
public:
    // Constructor
    Shape(string name, ostream &os) : m_name(name), m_os(os) { m_os << "Construyendo Shape: " << m_name << endl; }
    // Destructor
    virtual ~Shape()                  { m_os << "Destruyendo  Shape: " << m_name << endl; };
    // Metodo virtual puro
    virtual Area   GetArea() const = 0; 
    // Metodo virtual
    virtual string GetName() const { return m_name; };
    // Metodo virtual
    virtual string ToString() const { return m_name; };
};

ostream& operator<<(ostream& os, const Shape& shape);

#endif // __SHAPE_H__