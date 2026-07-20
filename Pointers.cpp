#include <iostream> // cout, cin, endl
#include <fstream>  // ofstream
#include "Pointers.h"
#include "util.h"
#include "array1.h"
#include "array2.h"
#include "array3.h"
#include "array4.h"
#include "X.h"

using namespace std;

template <typename T>
void Print(T n) { cout << "valor: " << n << endl; }

template <typename T>
void Print2(T &n) { cout << "valor: " << n << endl; }

template <typename T>
void AddX(T &n, T val1, T val2) { n += val1 + val2; }

template <typename T>
void Square(T &n) { n *= n; }

// C/C++/C++11...
void f1(TI  n)  { ++n; }

// C++98, C++11...
// n es un parametro por referencia
// n se sobrepone solo con variables del mismo tipo
void f2(TI &n)  { ++n; } 

// C++11 ...
void f3(const TI &ri) {}

// C, C++98, C++11...
// Esto es un paso de puntero por valor
// La variable o contenido apuntado por pi
// nunca pasa realmente pero se puede sobreescribir
// porque el puntero lo está apuntando
// y puedo acceder a él con *pi
/**
 * @brief Paso de puntero por valor: modifica el contenido pero no el puntero.
 *
 * El ++*pi si afecta a la variable original porque el puntero la apunta,
 * pero el pi = nullptr solo cambia la copia local del puntero.
 *
 * @param pi Puntero al entero a incrementar. La copia local se anula al final.
 */
void f4(TI *pi)  { ++*pi;  pi = nullptr; }

// C++98, C++11...
// Esto si es un paso de puntero por referencia
// Se puede sobreescribir el puntero
// rpi es una referencia
/**
 * @brief Paso de puntero por referencia: modifica el contenido y el puntero.
 *
 * A diferencia de f4, aqui rpi es una referencia al puntero original,
 * asi que el rpi = nullptr si anula el puntero del que llama.
 *
 * @param rpi Referencia al puntero. Se incrementa *rpi y luego se anula rpi.
 */
void f5(TI *&rpi) { ++*rpi;    rpi = nullptr; }

void f6(TI **ppi)  { ++**ppi;  ppi  = nullptr; }
void f7(TI **&rppi){ ++**rppi; rppi = nullptr; }

void DemoPointers1() {
  TI a = 10, b = 20, &ref = a;
  TI *p   = nullptr, *q; // q apunta a un lugar impredecible
  TI **pp = nullptr;

  p  = &a;   q = &b;
  pp = &p;

  cout << "a: " << a << "\t&a: " << &a << endl;
  cout << "p: " << p << "\t&p: " << &p << endl;
  cout << "b: " << b << "\t&b: " << &b << endl;
  cout << "q: " << q << "\t&q: " << &q << endl;
  
  cout << "*p: " << *p << endl;

  cout << "\nCaso #1\n";
  f1(a);   Print(a);
  f1(ref); Print(a);
  f1(5); 
  f1(a+10); 
  f1(*p);   Print(a);
  f1(**pp); Print(a);

  cout << "\nCaso #2\n";
  a = 20;    b = 20;
  f2(a);    Print(a);
  f2(ref);  Print(a);
  // f2(5); // error 
  // f2(a+10); // error
  f2( *p ); Print(a);
  f2(**pp); Print(a);

  cout << "\nCaso #3\n";
  a = 30;    b = 20;
  f3(a);    Print(a);
  f3(ref);  Print(a);
  f3(5);    Print(a);
  f3(a+10); Print(a);
  f3( *p ); Print(a);
  f3(**pp); Print(a);

  cout << "\nCaso #4\n";
  a = 40;    b = 20;
  f4(&a);    Print(a);
  f4(&ref);  Print(a);
  f4(p);     Print(a);
  f4(*pp);   Print(a);

  cout << "\nCaso #5\n";
  a = 50;    b = 20;
  // f4(&a);    Print(a); // error
  // f4(&ref);  Print(a); // error
  f5(p);     Print(a);
  cout << "p: " << p << endl;
  p = &a;
  f5(*pp);   Print(a);
  p = &a;

  cout << "\nCaso #6\n";
  a = 60;    b = 20;
  f6(&p);    Print(a);
  f6(pp);    Print(a);

  cout << "\nCaso #7\n";
  a = 70;    b = 20;
  f7(pp);    Print(a);
  // f7(&p);
  cout << "pp: " << pp << endl;
}


void DemoPointersVector1() {
    cout << "Nivel #1 (cachimbo mal linformado)\n";
    cout << "Problemas:\n";
    cout << "* Solo funciona para un tipo\n";
    cout << "* No se puede crear el arreglo en cualquier lugar (solo en el heap)\n";
    cout << "* Está imprimiendo siempre en pantalla (cout)\n";
    cout << "* Está leyendo siempre de teclado\n";
    cout << "* No tengo una funcion para aplicar una transformacion a los elementos\n";
    cout << "* No tengo una clase para encapsular los datos\n";

    TP *pArray1 = nullptr;
    size_t n;
    cout << "Ingrese el tamaño del vector: ";
    cin >> n;
    
    CreateArray1(pArray1, n);
    ReadArray1(pArray1, n);
    PrintArray1(pArray1, n);
    DeleteArray1(pArray1);
}

void DemoPointersVector2(){
    TI *pArray2 = nullptr;
    cout << "Nivel #2 (cachimbo mejor informado)\n";
    cout << "Problemas:\n";
    cout << "* No se puede crear el arreglo en cualquier lugar (solo en el heap)\n";
    cout << "* No tengo una clase para encapsular los datos\n";
    size_t n2;
    cout << "Ingrese el tamaño del vector: ";
    cin >> n2;
    
    CreateArray2(pArray2, n2);
    ReadArray2(pArray2, n2, cin);
    cout << "Vector original: ";
    PrintArray2(pArray2, n2, cout);
    
    cout << "Funcion #1 aplicada:\n";
    ApplyFunctionToAll2(pArray2, n2, Print2<TI>);
    PrintArray2(pArray2, n2, cout);
    
    cout << "Funcion #2 aplicada: ";
    ApplyFunctionToAll2(pArray2, n2, Square<TI>);
    cout << "Vector transformado #3: ";
    PrintArray2(pArray2, n2, cout);

    ofstream ofs("output2.txt");
    PrintArray2(pArray2, n2, ofs);
    ofs.close();

    DeleteArray2(pArray2);
}

void DemoPointersVector3(){
    cout << "Nivel #3 (cachimbo con clase)\n";
    cout << "Ingrese el tamaño del vector: ";
    Array3<TI> array3;
    array3.Read(cin);
    array3.Print(cout);
    array3.ApplyFunctionToAll(Print2<TI>);
    array3.ApplyFunctionToAll(Square<TI>);
    array3.Print(cout);
    ofstream ofs("output3.txt");
    array3.Print(ofs);
    ofs.close();
}

void DemoPointersVector4(){
    cout << "Nivel #4 (cachimbo respetable)\n";
    Array4<TI> array4;
    ifstream ifs("output3.txt");
    array4.Read(ifs);
    cout << "Aplicando Square\n";
    array4.ApplyFunctionToAll(Square<TI>);

    cout << "Imprimiendo en el cout\n";
    array4.ApplyFunctionToAll(Print4<TI>, cout);
    cout << endl;

    cout << "Sumándole 2 valores extras\n";
    array4.ApplyFunctionToAll(AddX<TI>, 5, 10);

    array4.ApplyFunctionToAll(Print4<TI>, cout);
    cout << endl;
}

void DemoPointersVector5(){
    cout << "Nivel #5 (cachimbo CS)\n";
    Array4<TI> array5;
    ifstream ifs("output3.txt");
    ifs >> array5;
    cout << "Aplicando Square\n";
    array5.ApplyFunctionToAll(Square<TI>);

    cout << "Imprimiendo en el cout\n";
    // array5.ApplyFunctionToAll(Print4<T>, cout);
    cout << array5;
    cout << endl;

    cout << "Sumándole 2 valores extras\n";
    array5.ApplyFunctionToAll(AddX<TI>, 5, 10);

    cout << array5 << endl;
    ofstream ofs("output5.txt");
    ofs << array5 ;
}

void DemoPointersMatrix1(){
}

void Print1(string str, X &x)  { cout << str << x.ToString() << endl; }
void Print1(string str, X *px) { Print1(str, *px); }

void Print2(string str, X *px) { cout << str << px->ToString() << endl; }
void Print2(string str, X &x)  { Print2(str, &x); }

void ModifyMember(X &x , TX X::*pAtt1, TX val) { x.*pAtt1   = val; }
void ModifyMember(X *px, TX X::*pAtt1, TX val) { px->*pAtt1 = val; }

void DemoPointersToMembers(){
    // Objeto estático
    X obj(20, 10);
    // Objeto Dinámico
    X *pObj = nullptr;
    pObj = new X(60, 15);
    X &rObj = *pObj;

    // Operaciones con un objeto estático
    obj.setA(25);
    obj.m_a = 40;
    // Operaciones con un objeto dinámico
    pObj->setA(30);
    pObj->m_a = 50;
    // Operaciones con una referencia a un objeto
    rObj.setA(35);
    rObj.m_a = 45;

    cout << "Manipuando miembros con punteros directos a cada miembro" << endl;
    TX *ptr = &obj.m_a;
    Print1("Antes   :", obj);
    *ptr = 100;
    Print1("Después :", obj);

    ptr = &pObj->m_a;
    Print1("Antes   :", *pObj);
    *ptr = 200;
    Print1("Después :", *pObj);

    cout << "Manipulando miembros con punteros a miembros" << endl;
    TX X::*apAtt[2] = {&X::m_a, &X::m_b};
    TX X::*pAtt = &X::m_a;
    Print1("Antes   :", obj);
    obj.*pAtt = 500;
    Print1("Después :", obj);

    Print1("Antes   :", *pObj);
    pObj->*pAtt = 600;
    Print1("Después :", *pObj);

    cout << "Manipulando miembros cambiantes ..." << endl;
    Print1("Antes   :", obj);
    ModifyMember(obj, &X::m_a, 700);
    Print1("Después1:", obj);
    ModifyMember(obj, pAtt, 750);
    Print1("Después2:", obj);
    ModifyMember(obj, &X::m_b, 800);
    Print1("Después3:", obj);

    cout << "Ahora con punteros a objetos dinámicos ..." << endl;
    auto x = 0;
    Print1("Antes   :", pObj);
    // apAtt[0] es un puntero a miembro que apunta a m_a
    ModifyMember(pObj, apAtt[x++], 900);
    Print1("Después1:", pObj);
    // apAtt[1] es un puntero a miembro que apunta a m_b
    ModifyMember(pObj, apAtt[x], 950);
    Print1("Después2:", pObj);

    cout << "Ahora llamar a metodos con punteros a miembros" << endl;
    TX (X::*pMet)() const = &X::Suma;
    Print1("Antes (estatico)  : ", obj);
    TX result = (obj.*pMet)();
    cout << "Suma: " << result << endl;
    Print1("Después(estatico) :", obj);

    Print1("Antes  (dinamico) :", pObj);
    result = (pObj->*pMet)();
    cout << "Suma: " << result << endl;
    Print1("Después(dinamico) :", pObj);

    cout << "Ahora con un arreglo de punteros a metodos ..." << endl;
    TX (X::*vpMet[4])() const = {&X::Suma    , &X::Resta, 
                                 &X::Producto, &X::Division};
    obj.setA(40); obj.setB(10);
    Print1("Antes  (estatico)  : ", obj);
    for(auto i = 0; i < 4; ++i) {
        result = (obj.*vpMet[i])();
        cout << "Resultado: " << result << endl;
    }

    pObj->setA(50); pObj->setB(20);
    Print1("Antes  (dinamico) :", pObj);
    for(auto i = 0; i < 4; ++i) {
        result = (pObj->*vpMet[i])();
        cout << "Resultado: " << result << endl;
    } 
    delete pObj;
}
