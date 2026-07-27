#ifndef THREAD_DEMO_H
#define THREAD_DEMO_H

#include <iostream>
#include <vector>

#include "../types.h"

using namespace std;


// Llena un vector con datos de prueba (0..999 cíclicos)
void llenarVector(vector<T1>& v, size_t tamanio);

// Suma secuencial de todos los elementos
long long sumarSecuencial(const vector<T1>& v);

// Suma concurrente usando 'numHilos' hilos
long long sumarConcurrente(const vector<T1>& v, unsigned int numHilos);

#endif