/**
 * @file cuda_add.cu
 * @brief Implementación de suma de vectores en GPU usando CUDA.
 */

#include "cuda_add.h"
#include "../types.h"
#include <cuda_runtime.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

/**
 * @brief Kernel CUDA que suma dos vectores elemento a elemento.
 *
 * @param a Vector de entrada (device) - primer sumando
 * @param b Vector de entrada (device) - segundo sumando
 * @param c Vector de salida (device) - resultado (a + b)
 * @param n Número de elementos de los vectores
 *
 * @note Cada hilo procesa un elemento: c[i] = a[i] + b[i].
 *       Se lanza con bloques de 256 hilos y la cantidad de bloques
 *       necesaria para cubrir n elementos.
 */


// Prototipo del kernel
__global__ void vectorAddKernel(const F1* a, const F1* b, F1* c, T1 n);

// Definición del kernel
__global__ void vectorAddKernel(const F1* a, const F1* b, F1* c, T1 n) {
    T1 i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

// Función que realiza la suma en GPU (recibe punteros y tamaño)
extern "C" void vectorAdd(const F1* a, const F1* b, F1* c, T1 n) {
    F1 *d_a, *d_b, *d_c;
    size_t size = n * sizeof(F1);
    cudaError_t err;

    T1 threadsPerBlock = 256;
    T1 blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    err = cudaMalloc(&d_a, size);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMalloc d_a: " << cudaGetErrorString(err) << '\n';
        return;
    }
    err = cudaMalloc(&d_b, size);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMalloc d_b: " << cudaGetErrorString(err) << '\n';
        cudaFree(d_a);
        return;
    }
    err = cudaMalloc(&d_c, size);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMalloc d_c: " << cudaGetErrorString(err) << '\n';
        cudaFree(d_a);
        cudaFree(d_b);
        return;
    }

    err = cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMemcpy d_a: " << cudaGetErrorString(err) << '\n';
        goto cleanup;
    }
    err = cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMemcpy d_b: " << cudaGetErrorString(err) << '\n';
        goto cleanup;
    }

    // Lanzar kernel
    vectorAddKernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // Verificar error del kernel
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        std::cerr << "Error en el kernel: " << cudaGetErrorString(err) << '\n';
        goto cleanup;
    }

    err = cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMemcpy d_c: " << cudaGetErrorString(err) << '\n';
    }

cleanup:
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

// Función de demostración (sin parámetros)
extern "C" void demoVectorAdd() {
    const T1 N = 10;  // Tamaño pequeño para depuración
    F1 *a = new F1[N];
    F1 *b = new F1[N];
    F1 *c = new F1[N];

    // Valores fijos para verificar fácilmente
    for (T1 i = 0; i < N; ++i) {
        a[i] = static_cast<F1>(i);
        b[i] = static_cast<F1>(i * 2);
    }

    std::cout << "Llamando a vectorAdd con " << N << " elementos en la GPU...\n";
    vectorAdd(a, b, c, N);

    std::cout << "Primeros " << N << " resultados:\n";
    for (T1 i = 0; i < N; ++i) {
        std::cout << a[i] << " + " << b[i] << " = " << c[i] << '\n';
    }

    delete[] a;
    delete[] b;
    delete[] c;
}