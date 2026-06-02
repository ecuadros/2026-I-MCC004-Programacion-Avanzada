#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <functional>
#include <iostream>
#include <utility>
#include <cassert>
#include "types.h"

using namespace std;

template <typename T>
void Print4(T &n, ostream &os) { os << n << " "; }

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0, m_cols = 0;
    public:
        Matrix1(size_t rows=0, size_t cols=0) : m_rows(rows), m_cols(cols) { } // constructor
        Matrix1(Matrix1 &other); // copy constructor
        Matrix1& operator=(Matrix1 &other);

        Matrix1(Matrix1 &&other); // move constructor
        Matrix1& operator=(Matrix1 &&other);

        ~Matrix1()     { Destroy(); }
        void     Create();
        istream &Read(istream &is);
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&& ...args);
        ostream &Print(ostream &os);
        void Destroy();
        //add methods
        Matrix1 operator*(const Matrix1 &other) const;
        Matrix1 operator+(const Matrix1 &other) const;
        Matrix1 operator-(const Matrix1 &other) const;
        Matrix1 operator*(T value) const;
        

};

template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

// copy constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &other) : m_rows(other.m_rows), m_cols(other.m_cols) {
    if (m_rows > 0 && m_cols > 0) {
        Create(); 
        for(size_t i = 0; i < m_rows; ++i) {
            for(size_t j = 0; j < m_cols; ++j) {
                m_pMat[i][j] = other.m_pMat[i][j];
            }
        }
    }
}

template <typename T>
Matrix1<T>& Matrix1<T>::operator=(Matrix1<T> &other) {
    if (this == &other) return *this;
    Destroy();
    
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    
    if (m_rows > 0 && m_cols > 0) {
        Create();
        for(size_t i = 0; i < m_rows; ++i) {
            for(size_t j = 0; j < m_cols; ++j) {
                m_pMat[i][j] = other.m_pMat[i][j]; // Copiamos valor por valor
            }
        }
    }
    return *this;
}

// Move constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &&other) {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

template <typename T>
Matrix1<T>& Matrix1<T>::operator=(Matrix1<T> &&other){
    if(this == &other) return *this;
    Destroy();
    m_pMat = exchange(other.m_pMat, nullptr);
    m_cols = exchange(other.m_cols, 0);
    m_rows = exchange(other.m_rows, 0);
    return *this;
}


template <typename T>
istream &Matrix1<T>::Read(istream &is){
    Destroy();
    is >> m_rows >> m_cols;
    Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            is >> m_pMat[i][j];
    return is;
};

template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args){
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            func(m_pMat[i][j], forward<Args>(args)...);
};

template <typename T>
ostream &Matrix1<T>::Print(ostream &os){
    os << m_rows << " " << m_cols << endl;
    for(size_t i = 0 ; i < m_rows ; ++i){
        for(size_t j = 0 ; j < m_cols ; ++j)
            os << m_pMat[i][j] << " ";
        os << endl;
    }
    return os;
};

template <typename T>
void Matrix1<T>::Destroy(){
    if(m_pMat == nullptr) return;
    for(size_t i = 0 ; i < m_rows ; ++i)
        delete[] m_pMat[i];
    delete[] m_pMat;
    m_pMat = nullptr;
    m_rows = m_cols = 0;
};

template <typename T>
istream &operator>>(istream &is, Matrix1<T> &matrix){
    return matrix.Read(is);
};

template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &matrix){
    matrix.Print(os);
    return os;
};

template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1 &other) const {
    assert(m_rows == other.m_rows && m_cols == other.m_cols); // filtro de dimensiones
    Matrix1<T> result(m_rows, m_cols);
    result.Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
    return result;
};

template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const {
    assert(m_rows == other.m_rows && m_cols == other.m_cols); // filtro de dimensiones
    Matrix1<T> result(m_rows, m_cols);
    result.Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
    return result;
};

template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) const{
    Matrix1<T> result(m_rows, m_cols);
    result.Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] * value;
    return result;
};

template<typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1 &other) const {
    assert(m_cols == other.m_rows); // filtro de dimensiones
    Matrix1<T> result(m_rows, other.m_cols);
    result.Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < other.m_cols ; ++j){
            result.m_pMat[i][j] = 0;
            for(size_t k = 0 ; k < m_cols ; ++k)
                result.m_pMat[i][j] += m_pMat[i][k] * other.m_pMat[k][j];
        }
    return result;
};

/*
template <typename T>
Matrix1<T> Matrix1<T>::operator=(Matrix1&& other){
    if(this == &other) return *this;
    Destroy();
    m_pMat = exchange(other.m_pMat, nullptr);
    m_cols = exchange(other.m_cols, 0);
    m_rows = exchange(other.m_rows, 0);
    return *this;
};
*/



void DemoMatrix();



#endif // __MATRIX_H__