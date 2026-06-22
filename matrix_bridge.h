#ifndef __MATRIX_BRIDGE_H__
#define __MATRIX_BRIDGE_H__

#include "matrix1.h"
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;


class MatrixBridge {
private:
    Matrix1<double> mat;
    
public:
    MatrixBridge() {}
    // Cargar matriz desde archivo
    bool load(const string& file) {
        ifstream ifs(file);
        if (!ifs.is_open()) return false;
        mat.Read(ifs); 
        return true;
    }
    
    // Guardar matriz a archivo
    bool save(const string& file) {
        ofstream ofs(file);
        if (!ofs.is_open()) return false;
        ofs << mat;
        return true;
    }
    
    // Obtener como string
    string toString() { 
        stringstream ss;
        ss << mat;
        return ss.str();
    }
};

// Funciones para Python (estilo C)
extern "C" {
    MatrixBridge* create_bridge() {
        return new MatrixBridge();
    }
    
    void destroy_bridge(MatrixBridge* b) {
        delete b;
    }
    
    bool bridge_load(MatrixBridge* b, const char* file) {//
        return b->load(file);
    }
    
    const char* bridge_toString(MatrixBridge* b) {
        static string result;
        result = b->toString();
        return result.c_str();
    }
    // Función que suma dos matrices
    const char* suma_matrices(const char* file1, const char* file2);
    const char* resta_matrices(const char* file1, const char* file2);
    const char* multi_escalar(const char* file, double scalar);
    const char* multi_matrices(const char* file1, const char* file2);
    const char* multi_matrices_parallel(const char* file1, const char* file2, unsigned int numThreads);}

#endif

// Función auxiliar para multiplicar un rango de filas
void multiplyRange(const Matrix1<double>& A, const Matrix1<double>& B,
                   Matrix1<double>& C, size_t start, size_t end) {
    size_t colsA = A.getCols();
    size_t colsB = B.getCols();
    for (size_t i = start; i < end; ++i) {
        for (size_t j = 0; j < colsB; ++j) {
            double sum = 0;
            for (size_t k = 0; k < colsA; ++k) {
                sum += A.get(i,k) * B.get(k,j);
            }
            C.set(i,j, sum);
        }
    }
}

extern "C" const char* multi_matrices_parallel(const char* file1, const char* file2, unsigned int numThreads) {
    static std::string result;
    Matrix1<double> A, B;
    std::ifstream f1(file1), f2(file2);
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error: no se pudieron abrir los archivos";
        return result.c_str();
    }
    A.Read(f1);
    B.Read(f2);
    if (A.getCols() != B.getRows()) {
        result = "Error: dimensiones incompatibles para multiplicación";
        return result.c_str();
    }
    Matrix1<double> C(A.getRows(), B.getCols());
    std::vector<std::thread> threads;
    size_t rows = A.getRows();
    size_t rowsPerThread = rows / numThreads;
    size_t remainder = rows % numThreads;
    size_t start = 0;
    for (unsigned int t = 0; t < numThreads; ++t) {
        size_t end = start + rowsPerThread + (t < remainder ? 1 : 0);
        threads.emplace_back(multiplyRange, std::cref(A), std::cref(B), std::ref(C), start, end);
        start = end;
    }
    for (auto& th : threads) th.join();
    std::stringstream ss;
    ss << C;
    result = ss.str();
    return result.c_str();
}