// Demo pequeño de CUDA: multiplicacion de matrices C = A * B en GPU.
//
// Compara la misma multiplicacion en CPU (tres loops secuenciales)
// contra la GPU (un hilo CUDA por cada elemento de C), verificando
// que los resultados coincidan y midiendo el tiempo de cada version.
//
// Compilar y ejecutar (requiere GPU NVIDIA y nvcc):
//   nvcc cuda_demo.cu -o cuda_demo
//   ./cuda_demo
//
// Las matrices se guardan como arreglos 1D en row-major:
// el elemento [fila][col] vive en indice fila*N + col — el mismo
// layout contiguo que usa Matrix1<T> internamente.

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <cuda_runtime.h>
#include "types.h"

// N x N: tamaño de las matrices del benchmark
static const TI N = 512;

/**
 * @brief Kernel CUDA: cada hilo calcula UN elemento de C.
 *
 * A diferencia de los threads de CPU (un hilo por fila que recorre
 * sus columnas), aqui se lanza un hilo por ELEMENTO: N*N hilos en
 * total. Cada hilo hace solo el producto punto de su fila de A con
 * su columna de B.
 *
 * Los hilos se organizan en una grilla 2D de bloques de 16x16.
 * blockIdx/blockDim/threadIdx dan a cada hilo su posicion (fila, col).
 *
 * @param A Matriz izquierda (N*N, row-major, en memoria de GPU).
 * @param B Matriz derecha  (N*N, row-major, en memoria de GPU).
 * @param C Matriz resultado (N*N, row-major, en memoria de GPU).
 * @param n Dimension de las matrices.
 */
__global__ void MatMulKernel(const T3F *A, const T3F *B, T3F *C, TI n)
{
    TI fila = blockIdx.y * blockDim.y + threadIdx.y;
    TI col  = blockIdx.x * blockDim.x + threadIdx.x;

    if (fila < n && col < n)
    {   T3F suma = 0.0f;
        for (TI k = 0; k < n; ++k)
            suma += A[fila * n + k] * B[k * n + col];
        C[fila * n + col] = suma;
    }
}

/**
 * @brief Multiplicacion de referencia en CPU: tres loops secuenciales.
 *
 * @param A Matriz izquierda (N*N, row-major).
 * @param B Matriz derecha  (N*N, row-major).
 * @param C Matriz resultado (N*N, row-major).
 * @param n Dimension de las matrices.
 */
static void MatMulCPU(const T3F *A, const T3F *B, T3F *C, TI n)
{
    for (TI i = 0; i < n; ++i)
        for (TI j = 0; j < n; ++j)
        {   T3F suma = 0.0f;
            for (TI k = 0; k < n; ++k)
                suma += A[i * n + k] * B[k * n + j];
            C[i * n + j] = suma;
        }
}

int main()
{
    const TI total = N * N;
    const size_t bytes = total * sizeof(T3F);

    // ── 1. Reservar y llenar las matrices en CPU ──────────────────────
    T3F *hA = (T3F *)malloc(bytes);
    T3F *hB = (T3F *)malloc(bytes);
    T3F *hC_cpu = (T3F *)malloc(bytes);   // resultado CPU
    T3F *hC_gpu = (T3F *)malloc(bytes);   // resultado GPU (copiado de vuelta)

    srand(42);
    for (TI i = 0; i < total; ++i)
    {   hA[i] = (T3F)(rand() % 10);
        hB[i] = (T3F)(rand() % 10);
    }

    // ── 2. Version CPU (cronometrada) ─────────────────────────────────
    auto t0 = std::chrono::steady_clock::now();
    MatMulCPU(hA, hB, hC_cpu, N);
    auto t1 = std::chrono::steady_clock::now();
    auto ms_cpu = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    // ── 3. Version GPU ────────────────────────────────────────────────
    // 3a. Reservar memoria en la GPU
    T3F *dA, *dB, *dC;
    cudaMalloc(&dA, bytes);
    cudaMalloc(&dB, bytes);
    cudaMalloc(&dC, bytes);

    // 3b. Copiar A y B de CPU a GPU
    cudaMemcpy(dA, hA, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, hB, bytes, cudaMemcpyHostToDevice);

    // 3c. Lanzar el kernel: grilla 2D de bloques de 16x16 hilos
    dim3 hilosPorBloque(16, 16);
    dim3 bloques((N + 15) / 16, (N + 15) / 16);

    t0 = std::chrono::steady_clock::now();
    MatMulKernel<<<bloques, hilosPorBloque>>>(dA, dB, dC, N);
    cudaDeviceSynchronize();              // esperar a que la GPU termine
    t1 = std::chrono::steady_clock::now();
    auto ms_gpu = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    // 3d. Copiar el resultado de GPU a CPU
    cudaMemcpy(hC_gpu, dC, bytes, cudaMemcpyDeviceToHost);

    // ── 4. Verificar que CPU y GPU dieron lo mismo ────────────────────
    TI errores = 0;
    for (TI i = 0; i < total; ++i)
        if (hC_cpu[i] != hC_gpu[i])
            ++errores;

    // ── 5. Reporte ────────────────────────────────────────────────────
    printf("MatMul %dx%d: %d hilos CUDA (%d bloques de 16x16)\n",
           N, N, total, bloques.x * bloques.y);
    printf("  CPU (1 hilo):       %ld ms\n", (long)ms_cpu);
    printf("  GPU (%d hilos): %ld ms\n", total, (long)ms_gpu);
    if (ms_gpu > 0)
        printf("  Speedup: x%.1f\n", (T5)ms_cpu / ms_gpu);
    printf("  Verificacion: %s (%d diferencias)\n",
           errores == 0 ? "IGUALES" : "DIFERENTES!", errores);
    printf("  Ejemplo: C[0] = %.1f\n", hC_gpu[0]);

    // ── 6. Liberar memoria ────────────────────────────────────────────
    cudaFree(dA); cudaFree(dB); cudaFree(dC);
    free(hA); free(hB); free(hC_cpu); free(hC_gpu);
    return 0;
}
