#include <iostream>
#include <thread>

using namespace std;

void tarea1()
{
    cout << "El hilo 1 ejecuta la primera tarea." << endl;
}

void tarea2()
{
    cout << "El hilo 2 ejecuta la segunda tarea." << endl;
}

int main()
{
    cout << "Inicio del programa." << endl;

    thread hilo1(tarea1);
    thread hilo2(tarea2);

    hilo1.join();
    hilo2.join();

    cout << "Fin del programa." << endl;

    return 0;
}