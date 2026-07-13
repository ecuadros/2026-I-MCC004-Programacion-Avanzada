#include <cstdio>
#include <cuda_runtime.h>

// Función que se ejecuta en la GPU
__global__ void sumar()
{
    int a = 5;
    int b = 3;

    printf("Suma realizada en la GPU: %d\n", a + b);
}

int main()
{
    sumar<<<1, 1>>>();

    cudaDeviceSynchronize();

    return 0;
}