// Ejemplo pequeño CUDA: suma de vectores C = A + B en la GPU.
//
// Requiere GPU NVIDIA y el CUDA Toolkit (nvcc).
// Si no hay GPU local, se puede correr en Google Colab:
//   1. Runtime -> Change runtime type -> GPU
//   2. %%writefile cuda_demo.cu   (pegar este archivo)
//   3. !nvcc cuda_demo.cu -o cuda_demo && ./cuda_demo
//
// Compilar local:
//   nvcc cuda_demo.cu -o cuda_demo
//   ./cuda_demo

#include <cstdio>
#include <cuda_runtime.h>
#include "types.h"

// __global__ marca la funcion como kernel: se ejecuta en la GPU,
// una copia por hilo. Cada hilo calcula UN elemento del resultado.
__global__ void VecAdd(const T3F *a, const T3F *b, T3F *c, TI n)
{
    // indice global del hilo: bloque * tamano_bloque + hilo_dentro_del_bloque
    TI i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)              // los hilos sobrantes del ultimo bloque no hacen nada
        c[i] = a[i] + b[i];
}

int main()
{
    const TI n = 1 << 20;   // ~1 millon de elementos
    size_t bytes = n * sizeof(T3F);

    // 1. Memoria en el host (CPU)
    T3F *h_a = new T3F[n], *h_b = new T3F[n], *h_c = new T3F[n];
    for (TI i = 0; i < n; ++i) { h_a[i] = i; h_b[i] = 2 * i; }

    // 2. Memoria en el device (GPU)
    T3F *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, bytes);
    cudaMalloc(&d_b, bytes);
    cudaMalloc(&d_c, bytes);

    // 3. Copiar entradas CPU -> GPU
    cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, bytes, cudaMemcpyHostToDevice);

    // 4. Lanzar el kernel: n hilos repartidos en bloques de 256
    TI threadsPerBlock = 256;
    TI blocks = (n + threadsPerBlock - 1) / threadsPerBlock;
    VecAdd<<<blocks, threadsPerBlock>>>(d_a, d_b, d_c, n);
    cudaDeviceSynchronize();

    // 5. Copiar resultado GPU -> CPU y verificar
    cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);

    bool ok = true;
    for (TI i = 0; i < n; ++i)
        if (h_c[i] != h_a[i] + h_b[i]) { ok = false; break; }

    printf("VecAdd con %d elementos, %d bloques x %d hilos: %s\n",
           n, blocks, threadsPerBlock, ok ? "OK" : "ERROR");
    printf("Ejemplo: c[10] = %.1f (esperado %.1f)\n", h_c[10], h_a[10] + h_b[10]);

    // 6. Liberar memoria
    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    delete[] h_a; delete[] h_b; delete[] h_c;
    return 0;
}
