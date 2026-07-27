#ifndef __VECTOR_ADD_H__
#define __VECTOR_ADD_H__

#include "../types.h"   // F1 y T1

/**
 * @brief Suma dos vectores en la GPU.
 *
 * @param a Vector de entrada (host) - primer sumando
 * @param b Vector de entrada (host) - segundo sumando
 * @param c Vector de salida (host)   - resultado (a + b)
 * @param n Número de elementos de los vectores
 *
 * @note Esta función realiza la suma en la GPU usando CUDA.
 *       Requiere que el dispositivo CUDA esté disponible y que
 *       el código se haya compilado con la arquitectura correcta
 *       (ver -arch=sm_XX en el Makefile).
 */

#ifdef __cplusplus
extern "C" {
#endif

//void vectorAdd(const F1* a, const F1* b, F1* c, T1 n);

/**
 * @brief Función de demostración: crea vectores de prueba,
 *        llama a vectorAdd y muestra los primeros 10 resultados.
 *
 * @note Los vectores se inicializan con valores fijos (0..9 y 0..18)
 *       para facilitar la verificación. Si el kernel funciona,
 *       deberían verse resultados como: 0, 3, 6, 9, ...
 */

void demoVectorAdd(T1 N);

#ifdef __cplusplus
}
#endif

#endif