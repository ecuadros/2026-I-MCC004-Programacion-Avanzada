#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <iostream>
#include <sstream>
#include "types.h"

using namespace std;

class Complex{
private:
    RealType m_real;
    ImagType m_imag;

public:
    // Constructor:
    // 1. Se llama igual que la clase
    // 2. No tiene tipo de retorno
    // 3. Puede tener sobrecarga
    // 4. Se llama de forma automatica al declarar un objeto
    // 5. Un constructor puede llamar a otro constructor
    // 6. 
    Complex(RealType real = 0, ImagType imag = 0){
        m_real = real;
        m_imag = imag;
    }
    Complex(Complex &other){
        m_real = other.GetReal();
        m_imag = other.GetImag();
    }
    // Destructor
    // 1. Se llama igual que la clase pero con el simbolo ~
    // 2. No tiene tipo de retorno
    // 3. No puede tener sobrecarga
    // 4. Se llama de forma automatica al destruir un objeto o
    //    cuando el objeto sale del ambito {} donde fue creado
    // 5. Siempre es recomendable que sea virtual
    virtual ~Complex(){

    }

    void SetReal(RealType real){ m_real = real; }
    void SetImag(ImagType imag){ m_imag = imag; }
    RealType GetReal() const{ return m_real; }
    ImagType GetImag() const{ return m_imag; }

    Complex operator+(const Complex& other) const{
        return Complex(m_real + other.GetReal(), m_imag + other.GetImag());
    }
    Complex operator-(const Complex& other) const{
        return Complex(m_real - other.GetReal(), m_imag - other.GetImag());
    }
    Complex operator*(const Complex& other) const{
        return Complex(m_real * other.GetReal() - m_imag * other.GetImag(), m_real * other.GetImag() + m_imag * other.GetReal());
    }
    Complex operator/(const Complex& other) const{
        return Complex((m_real * other.GetReal() + m_imag * other.GetImag()) / (other.GetReal() * other.GetReal() + other.GetImag() * other.GetImag()), (m_imag * other.GetReal() - m_real * other.GetImag()) / (other.GetReal() * other.GetReal() + other.GetImag() * other.GetImag()));
    }
    bool operator==(const Complex& other) const{
        return m_real == other.GetReal() && m_imag == other.GetImag();
    }
    bool operator!=(const Complex& other) const{
        return m_real != other.GetReal() || m_imag != other.GetImag();
    }
    bool operator<(const Complex& other) const{
        return m_real < other.GetReal() && m_imag < other.GetImag();
    }
    bool operator>(const Complex& other) const{
        return m_real > other.GetReal() && m_imag > other.GetImag();
    }
    Complex operator<=(const Complex& other) const{
        return m_real <= other.GetReal() && m_imag <= other.GetImag();
    }
    Complex operator>=(const Complex& other) const{
        return m_real >= other.GetReal() && m_imag >= other.GetImag();
    }
    Complex operator++() const{ return Complex(m_real + 1, m_imag + 1); }
    Complex operator++(int) const{  return Complex(m_real + 1, m_imag + 1); }
    Complex operator--() const{ return Complex(m_real - 1, m_imag - 1); }
    Complex operator--(int) const{ return Complex(m_real - 1, m_imag - 1); }
    
    Complex operator+() const{
        return Complex(m_real, m_imag);
    }
    Complex operator-() const{
        return Complex(-m_real, -m_imag);
    }
    Complex operator~() const{
        return Complex(m_imag, m_real);
    }
    Complex operator!() const{
        return Complex(!m_real, !m_imag);
    }
    Complex operator&(const Complex& other) const;
    Complex operator|(const Complex& other) const;
    Complex operator^(const Complex& other) const;
    Complex operator<<(const Complex& other) const;
    Complex operator>>(const Complex& other) const;
    Complex operator,(const Complex& other) const;
    Complex operator->() const;
    Complex operator->*(const Complex& other) const;
    Complex operator[](const Complex& other) const;
    Complex operator()(const Complex& other) const; 
    
    string ToString() const {
        ostringstream oss;
        oss << m_real << " + " << m_imag << "i";
        return oss.str();
    }
};

ostream& operator<<(ostream& os, const Complex& c);

void DemoComplex();

#endif // __COMPLEX_H__