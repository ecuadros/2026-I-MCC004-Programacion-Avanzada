#include <iostream>
#include <math.h>
#include "complex.h"
#include "functions.h"
#include "util.h"
#include "BitSigno.h"
#include "polimorfismo.h"
#include "Pointers.h"
#include "BitSigno.h"
#include "matrix1.h"

using namespace std;

void DemoMatrix1()
{   cout<<"Demo Matrix1" << endl;
    Matrix1<int> m;
    cout<<"Datos de la matriz:" << endl;
    cin>>m;
    cout<<endl;
    cout<<"Matriz<<:" << endl;
    cout<<m;
    cout<<endl;
    cout<<"El resultado sería:"<<endl;
    m.ApplyFunctionToAll(Print4<int>,cout);
    cout<<endl;
}

void DemoMatrix13();
void DemoMatrix14();

// Ernesto Cuadros
// g++ -std=c++2b main.cpp util.cpp -o main
int main() {
    // DemoFunctions();
    // DemoComplex();
    // DemoPolimorfismo();
    // BitSigno();
    // DemoPointers1();
    // DemoPointersVector1();
    // DemoPointersVector2();
    // DemoPointersVector3();
    // DemoPointersVector4();
    // DemoPointersVector5();
    
    DemoMatrix14();
    return 0;
}

void DemoMatrix13()
{
    Matrix1<int> m1, m2, m3, m4;

    cout << "Ingrese m2:" << endl;
    cin >> m2;

    cout << "Ingrese m3:" << endl;
    cin >> m3;

    cout << "Ingrese m4:" << endl;
    cin >> m4;

    m1 = 5 * m2 + m3 * m4;

    cout << endl;
    cout << "Resultado:" << endl;
    cout << m1;
}

void DemoMatrix14()
{
    size_t filas, columnas;

    cout << "Ingrese numero de filas: ";
    cin >> filas;

    cout << "Ingrese numero de columnas: ";
    cin >> columnas;

    Matrix1<int> m(filas, columnas);

    cout << endl;
    cout << "Ingrese los valores de la matriz:" << endl;

    for(size_t i = 0 ; i < filas ; ++i)
    {
        for(size_t j = 0 ; j < columnas ; ++j)
        {
            cout << "m[" << i << "][" << j << "] = ";
            cin >> m[i][j];
        }
    }

    cout << endl;
    cout << "Matriz original:" << endl;
    cout << m;

    size_t fila, columna;
    int valor;

    cout << endl;
    cout << "Ingrese fila a modificar: ";
    cin >> fila;

    cout << "Ingrese columna a modificar: ";
    cin >> columna;

    cout << "Ingrese nuevo valor: ";
    cin >> valor;

    m[fila][columna] = valor;

    if(fila < filas && columna < columnas)
{
    m[fila][columna] = valor;

    cout << endl;
    cout << "Matriz modificada usando m[fila][columna] = valor:" << endl;
    cout << m;
}
else
{
    cout << endl;
    cout << "La posicion ingresada no existe en la matriz." << endl;
}
}