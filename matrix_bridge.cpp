#include "matrix_bridge.h"
#include <sstream>
#include <thread>
#include <vector>

extern "C" const char* suma_matrices(const char* file1, const char* file2) {
    static std::string result;
    Matrix1<double> mat1, mat2;
    std::ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error: no se pudieron abrir los archivos";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);
    
    // Verificar dimensiones
    if (mat1.getRows() != mat2.getRows() || mat1.getCols() != mat2.getCols()) {
        result = "Error: dimensiones incompatibles para la suma y resta";
        return result.c_str();
    }
    
    Matrix1<double> suma = mat1 + mat2;
    std::stringstream ss;
    ss << suma;
    result = ss.str();
    return result.c_str();

}

extern "C" const char* resta_matrices(const char* file1, const char* file2) {
    static std::string result;
    Matrix1<double> mat1, mat2;
    std::ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error: no se pudieron abrir los archivos";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);
    
    // Verificar dimensiones
    if (mat1.getRows() != mat2.getRows() || mat1.getCols() != mat2.getCols()) {
        result = "Error: dimensiones incompatibles para la suma y resta";
        return result.c_str();
    }
    
    Matrix1<double> resta = mat1 - mat2;
    std::stringstream ss;
    ss << resta;
    result = ss.str();
    return result.c_str();

}

extern "C" const char* multi_escalar(const char* file, double scalar) {
    static std::string result;
    Matrix1<double> mat;
    std::ifstream f(file);
    
    if (!f.is_open()) {
        result = "Error: no se pudo abrir el archivo";
        return result.c_str();
    }
    
    mat.Read(f);
    Matrix1<double> resultado = mat * scalar;
    std::stringstream ss;
    ss << resultado;
    result = ss.str();
    return result.c_str();
}


extern "C" const char* multi_matrices(const char* file1, const char* file2) {
    static std::string result;
    Matrix1<double> mat1, mat2;
    std::ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        result = "Error: no se pudieron abrir los archivos";
        return result.c_str();
    }
    
    mat1.Read(f1);
    mat2.Read(f2);
    
    if (mat1.getCols() != mat2.getRows()) {
        result = "Error: dimensiones incompatibles para multiplicación (columnas de A vs filas de B)";
        return result.c_str();
    }
    
    Matrix1<double> producto = mat1.operatorX(mat2);
    std::stringstream ss;
    ss << producto;
    result = ss.str();
    return result.c_str();
}

