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
    m.ApplyFunctionToAll(PrintMatrix<int>, cout);
    cout<<endl;
}
// Ernesto Cuadros

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
    
    DemoPointersMatrix1();
    
    return 0;
}
