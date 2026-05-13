#include <iostream>
#include "Pointers.h"
#include "matrix1.h"

using namespace std;
template <typename T>
void Square(T &n)
{
    n *= n;
}
void DemoPointersMatrix1(){
    cout << "Demo Matrix1\n";
    Matrix1<int> matrix;
    cout << "Ingrese filas y columnas:\n";
    matrix.Read(cin);
    cout << "Matriz original:\n";
    matrix.Print(cout);
    cout << "Aplicando Square\n";
    matrix.ApplyFunctionToAll(Square<int>);
    matrix.Print(cout);
    cout << "Imprimiendo con funcion:\n";
    matrix.ApplyFunctionToAll(Print4<int>, cout);
    cout << endl;
}
