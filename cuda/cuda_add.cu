#include "cuda_add.h"
#include <cuda_runtime.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Kernel: cada hilo suma un elemento
__global__ void vectorAddKernel(const F1* a, const F1* b, F1* c, T1 n) {
    T1 i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

void vectorAdd(const F1* a, const F1* b, F1* c, T1 n) {
    F1 *d_a, *d_b, *d_c;
    size_t size = n * sizeof(F1);
    cudaError_t err;

    // --- DECLARAR AQUÍ (antes de cualquier goto) ---
    T1 threadsPerBlock = 256;
    T1 blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Reservar memoria en la GPU
    err = cudaMalloc(&d_a, size);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMalloc d_a: " << cudaGetErrorString(err) << '\n';
        return;   // mejor que goto, pero si quieres usar goto, asegura que no saltes sobre variables
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

    // Copiar datos al dispositivo
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

    // Lanzar kernel (ya tenemos threadsPerBlock y blocksPerGrid definidos)
    vectorAddKernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // Copiar resultado de vuelta al host
    err = cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        std::cerr << "Error cudaMemcpy d_c: " << cudaGetErrorString(err) << '\n';
    }

cleanup:
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

void demoVectorAdd() {
    const T1 N = 1000000;   // 1 millón
    F1 *a = new F1[N];
    F1 *b = new F1[N];
    F1 *c = new F1[N];

    // Inicializar con números aleatorios
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (T1 i = 0; i < N; ++i) {
        a[i] = static_cast<F1>(std::rand()) / RAND_MAX;
        b[i] = static_cast<F1>(std::rand()) / RAND_MAX;
    }

    std::cout << "Llamando a vectorAdd con " << N << " elementos en la GPU...\n";
    vectorAdd(a, b, c, N);

    // Mostrar primeros 10 resultados
    std::cout << "Primeros 10 resultados:\n";
    for (T1 i = 0; i < 10; ++i) {
        std::cout << a[i] << " + " << b[i] << " = " << c[i] << '\n';
    }

    delete[] a;
    delete[] b;
    delete[] c;
}