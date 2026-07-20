#ifndef __VECTOR_ADD_H__
#define __VECTOR_ADD_H__

#include "../types.h"   // F1 y T1

/**
 * @brief Suma dos vectores en la GPU.
 * @param a Vector de entrada (host)
 * @param b Vector de entrada (host)
 * @param c Vector de salida (host)
 * @param n Número de elementos
 */
void vectorAdd(const F1* a, const F1* b, F1* c, T1 n);

/**
 * @brief Función de demostración: crea vectores, llama a vectorAdd y muestra resultados.
 */
void demoVectorAdd();

#endif //__VECTOR_ADD_H__