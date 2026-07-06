#include "matrix_bridge.h"
#include <sstream>
#include <thread>
#include <vector>

using namespace std;

extern "C" const char* suma_matrices(const char* file1, const char* file2) {
    static string result;
    Matrix1<double> mat1, mat2;
    ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);

    if (mat1.getRows() != mat2.getRows() || mat1.getCols() != mat2.getCols()) {
        result = "Error";
        return result.c_str();
    }
    
    Matrix1<double> suma = mat1 + mat2;
    stringstream ss;
    ss << suma;
    result = ss.str();
    return result.c_str();
}

extern "C" const char* resta_matrices(const char* file1, const char* file2) {
    static string result;
    Matrix1<double> mat1, mat2;
    ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);
    
    if (mat1.getRows() != mat2.getRows() || mat1.getCols() != mat2.getCols()) {
        result = "Error";
        return result.c_str();
    }
    
    Matrix1<double> resta = mat1 - mat2;
    stringstream ss;
    ss << resta;
    result = ss.str();
    return result.c_str();
}

extern "C" const char* multi_escalar(const char* file, double scalar) {
    static string result;
    Matrix1<double> mat;
    ifstream f(file);
    
    if (!f.is_open()) {
        result = "Error";
        return result.c_str();
    }
    
    mat.Read(f);
    Matrix1<double> resultado = mat * scalar;
    stringstream ss;
    ss << resultado;
    result = ss.str();
    return result.c_str();
}


extern "C" const char* multi_matrices(const char* file1, const char* file2) {
    static string result;
    Matrix1<double> mat1, mat2;
    ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);
    
    if (mat1.getCols() != mat2.getRows()) {
        result = "Error";
        return result.c_str();
    }
    
    Matrix1<double> producto = mat1.operatorX(mat2);
    stringstream ss;
    ss << producto;
    result = ss.str();
    return result.c_str();
}

