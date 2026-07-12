#include <iostream>
#include "matrix1.h"

using namespace std;

int main()
{
    cout << "MATRIX1 CON NUMEROS ENTEROS" << endl;
    Matrix1<int> matrizEnteros(2, 2);
    matrizEnteros[0][0] = 1;
    matrizEnteros[0][1] = 2;
    matrizEnteros[1][0] = 3;
    matrizEnteros[1][1] = 4;
    cout << matrizEnteros << endl;
    cout << "\nMATRIX1 CON NUMEROS DECIMALES" << endl;

    Matrix1<float> matrizDecimales(2, 2);
    matrizDecimales[0][0] = 1.5f;
    matrizDecimales[0][1] = 2.7f;
    matrizDecimales[1][0] = 3.2f;
    matrizDecimales[1][1] = 4.8f;
    cout << matrizDecimales << endl;

    return 0;
}