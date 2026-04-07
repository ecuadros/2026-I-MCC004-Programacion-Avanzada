#include <iostream>  // cout
#include <fstream>   // ofstream
#include "complex.h"

using namespace std;

ostream& operator<<(ostream& os, const Complex& c){
    return os << c.ToString();
}

void DemoComplex(){
    Complex c1(1, 2);
    Complex c2(3, 4);
    Complex c3 = c1 + c2;
    Complex c4 = c1 - c2;
    Complex c5 = c1 * c2;
    Complex c6 = c1 / c2;
    Complex c7 = ++c1;
    Complex c8 = c1++;
    Complex c9 = --c1;
    Complex c10 = c1--;
    Complex c11 = +c1; //inutil
    Complex c12 = -c1;
    Complex c13 = (c1+c2)*(c3-c4)/(c5+c6);

    cout << "c1: " << c1 << endl;
    cout << "c2: " << c2 << endl;
    cout << "c3: " << c3 << endl;
    cout << "c4: " << c4 << endl;
    cout << "c5: " << c5 << endl;
    cout << "c6: " << c6 << endl;
    cout << "c7: " << c7 << endl;
    cout << "c8: " << c8 << endl;
    cout << "c9: " << c9 << endl;
    cout << "c10: " << c10 << endl;

    ofstream of("Test.txt");
    of << "c1: " << c1 << endl;
    of << "c2: " << c2 << endl;
}