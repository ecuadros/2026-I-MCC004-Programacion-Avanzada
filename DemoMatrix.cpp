#include <fstream>

#include "matrix1.h"

template <typename T>
void AddX(T &n, T val1) { n += val1; }

template <typename T>
void MultiConst(T &n, T value) { n *= value; }

void DemoMatrix(){
    cout << "Demo Matrix" << endl;
    cout << "Leyendo Matriz" << endl;
    Matrix1<TI> matrix1; 
    ifstream ifs("matrix1.txt");
    ifs >> matrix1;
    matrix1.Print(cout);
    cout << "Second way to Print" << endl;
    Print4(matrix1, cout);

    cout << "Probando ApplyFunctionToAll" << endl;
    cout << "Aplicando print" << endl;
    matrix1.ApplyFunctionToAll(Print4<TI>, cout);

    cout << "Aplicando funcion Add" << endl;
    cout << "Sumando 5 a cada elemento" << endl;
    matrix1.ApplyFunctionToAll(AddX<TI>, 5);
    matrix1.Print(cout);

    cout << "Aplicando funcion MultiConst" << endl;
    cout << "Multiplicando por 2" << endl;
    matrix1.ApplyFunctionToAll(MultiConst<TI>, 2);
    matrix1.Print(cout);
 
};