#include <iostream>
#include "BitSigno.h"
#include "util.h"

using namespace std;

       size_t size1() { return 0; }
unsigned char size2() { return 0; }

void BitSigno(){
    unsigned long i1 = size1()-1;
             long i2 = size1()-1;
    cout << "-1 en un size_t     : " << i1 << endl;
    cout << "-1 en un signed long: " << i2 << endl;

    unsigned char i3 = size2()-1;
             char i4 = size2()-1;
    cout << "-1 en un char       : " << (int)i3 << endl;
    cout << "-1 en un char       : " << (int)i4 << endl;

    int i = 5000;
    unsigned char i5 = i; // 136
             char i6 = i; // -120
    cout << "i5: " << (int)i5 << endl;
    cout << "i6: " << (int)i6 << endl;
}