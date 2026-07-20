#include "threadDemo.h"
#include <thread>
#include <algorithm>
#include <chrono>
void llenarVector(vector<T1>& v, size_t tamanio) {
    v.resize(tamanio);
    for (size_t i = 0; i < tamanio; ++i) {
        v[i] = static_cast<T1>(i % 1000);  // valores pequeños para evitar overflow
    }
}

long long sumarSecuencial(const vector<T1>& v) {
    long long suma = 0;
    for (T1 val : v) {
        suma += val;
    }
    return suma;
}

// Función auxiliar: suma un rango [inicio, fin)
static long long sumarRango(const vector<T1>& v, size_t inicio, size_t fin) {
    long long suma = 0;
    for (size_t i = inicio; i < fin; ++i) {
        suma += v[i];
    }
    return suma;
}

long long sumarConcurrente(const vector<T1>& v, unsigned int numHilos) {
    size_t tamanio = v.size();
    if (numHilos == 0) numHilos = 1;
    if (numHilos > tamanio) numHilos = static_cast<unsigned int>(tamanio);

    vector<thread> hilos;
    vector<long long> resultados(numHilos, 0);

    size_t bloque = tamanio / numHilos;
    size_t resto = tamanio % numHilos;

    for (unsigned int i = 0; i < numHilos; ++i) {
        size_t inicio = i * bloque + std::min<size_t>(i, resto);
        size_t fin = inicio + bloque + (i < resto ? 1 : 0);
        // Lanzar hilo que suma el rango y guarda en resultados[i]
        hilos.emplace_back([&v, inicio, fin, &resultados, i]() {
            resultados[i] = sumarRango(v, inicio, fin);
        });
    }

    // Esperar a todos los hilos
    for (auto& h : hilos) {
        if (h.joinable()) h.join();
    }

    // Sumar los resultados parciales
    long long total = 0;
    for (long long val : resultados) {
        total += val;
    }
    return total;
}