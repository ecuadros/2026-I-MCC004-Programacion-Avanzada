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
    bool load(const string& file) {
        ifstream ifs(file);
        if (!ifs.is_open()) return false;
        mat.Read(ifs); 
        return true;
    }
    
    bool save(const string& file) {
        ofstream ofs(file);
        if (!ofs.is_open()) return false;
        ofs << mat;
        return true;
    }
    
    string toString() { 
        stringstream ss;
        ss << mat;
        return ss.str();
    }
};

extern "C" {
    MatrixBridge* create_bridge() {//
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
    const char* suma_matrices(const char* file1, const char* file2);
    const char* resta_matrices(const char* file1, const char* file2);
    const char* multi_escalar(const char* file, double scalar);
    const char* multi_matrices(const char* file1, const char* file2);
}

#endif
