#include <iostream>
#include "Pointers.h"
#include "util.h"
#include "types.h"

using namespace std;

template <typename T>
void Print(T n) { cout << "valor: " << n << endl; }

void f1(TI  n)  { ++n;            }
void f2(TI &n)  { ++n;            }
// void f3(TI *n)  { ++(*n);         }
// void f4(TI **n) { ++(*n);         }

void DemoPointers1() {
  TI a = 10, b = 20;
  TI *p   = nullptr, *q; // q apunta a un lugar impredecible
  TI **pp = nullptr;

  p  = &a;   q = &b;
  pp = &p;

  cout << "a: " << a << "\t&a: " << &a << endl;
  cout << "p: " << p << "\t&p: " << &p << endl;
  cout << "b: " << b << "\t&b: " << &b << endl;
  cout << "q: " << q << "\t&q: " << &q << endl;
  
  cout << "*p: " << *p << endl;

  cout << "Caso #1\n";
  f1(a); Print(a);
  f1(5); 
  f1(a+10); 
  f1(*p);   Print(a);
  f1(**pp); Print(a);

  cout << "Caso #2\n";
  f2(a); Print(a);

}