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

    cout << "=============================" << endl;
    cout << "m2: " << endl;
    Matrix1<TI> m2;
    ifstream ifs2("m2.txt");
    ifs2 >> m2;
    m2.Print(cout);

    cout << "m3:" << endl;
    Matrix1<TI> m3;
    ifstream ifs3("m3.txt");
    ifs3 >> m3;
    m3.Print(cout);

    cout << "m4:" << endl;
    Matrix1<TI> m4;
    ifstream ifs4("m4.txt");
    ifs4 >> m4;
    m4.Print(cout);

    Matrix1<TI> m1;
    m1 = m2*5 + m3 * m4;
    m1.Print(cout);

    cout << "== Probando copy constructor ===" << endl;
    Matrix1<TI> m5=m1;
    m5.Print(cout);
    cout << "=======" << endl;
    m1.Print(cout);
    

    
    
 
};