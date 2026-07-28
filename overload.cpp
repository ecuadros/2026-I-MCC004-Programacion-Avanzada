#include <iostream>
#include <cstdlib>

using namespace std;

using TI = int;
using TF = double;

class Persona{
private:
    static constexpr size_t POOL_SIZE = 10;
    static char pool[];
    static bool ocupado[];

public:
    TI edad;
    TF peso;

    Persona(TI e, TF p): edad(e), peso(p){}
    ~Persona(){}

    static void* operator new(size_t);
    static void operator delete(void*);
    static void MostrarPool(ostream& os);
};

char Persona::pool[Persona::POOL_SIZE * sizeof(Persona)];
bool Persona::ocupado[Persona::POOL_SIZE] = {};

void* Persona::operator new(size_t){
    for (size_t i = 0; i < POOL_SIZE; i++){
        if (!ocupado[i]){
            ocupado[i] = true;
            return pool + i * sizeof(Persona);
        }
    }

    throw bad_alloc();
}

void Persona::operator delete(void* ptr){
    char* inicio = pool;
    char* p = static_cast<char*>(ptr);

    size_t indice = (p - inicio) / sizeof(Persona);

    ocupado[indice] = false;
}

ostream& operator<<(ostream& os, const Persona& p){
    os << "Edad: " << p.edad << ", Peso: " << p.peso;
    return os;
}

void Persona::MostrarPool(ostream& os){
    os << "Pool: ";

    for (size_t i = 0; i < POOL_SIZE; i++){
        os << (ocupado[i] ? "[X]" : "[ ]");
    }

    os << '\n';
}

void DemoOverload(){
    Persona::MostrarPool(cout);

    Persona* p1 = new Persona(25, 70.5);
    Persona::MostrarPool(cout);

    Persona* p2 = new Persona(30, 80);
    Persona::MostrarPool(cout);

    Persona* p3 = new Persona(40, 90);
    Persona::MostrarPool(cout);

    cout << "\nDirecciones:\n";
    cout << "p1 = " << p1 << '\n';
    cout << "p2 = " << p2 << '\n';
    cout << "p3 = " << p3 << '\n';

    cout << "\nEliminando p2\n";
    delete p2;
    Persona::MostrarPool(cout);

    cout << "\nCreando p4\n";
    Persona* p4 = new Persona(50, 100);
    Persona::MostrarPool(cout);
    cout << "p4 = " << p4 << '\n';

    delete p1;
    delete p3;
    delete p4;

    Persona::MostrarPool(cout);
}