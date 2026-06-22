#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <functional>
#include <iostream>
#include <cassert>
#include <utility>

using namespace std;

template <typename T>
void PrintMatriz(T &elem, size_t row, size_t col, ostream &os) {
    os << "[" << row << "][" << col << "] = " << elem << "  ";
}

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0, m_cols = 0;

    public:
        Matrix1()      { };
        Matrix1(Matrix1 &other) ;
        Matrix1(Matrix1 &&other); 
        ~Matrix1()     { Destroy(); }
        void     Create();
        istream &Read(istream &is);
        template <typename Func, typename... Args>
        //void ApplyFunctionToAll(Func func, Args&& ...args);
        void ApplyFunctionToAllMatriz(Func func, Args&&... args);
        ostream &Print(ostream &os);
        size_t   getRows() const { return m_rows; }
        size_t   getCols() const { return m_cols; }
        Matrix1(size_t rows, size_t cols); 
        Matrix1 operator+(const Matrix1 &other) const;
        Matrix1 operator-(const Matrix1 &other) const;
        Matrix1 operatorX(const Matrix1 &other) const; 
        Matrix1 operator*(T value) const;


        void Destroy();
};


template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

template <typename T>
Matrix1<T>::Matrix1(size_t rows, size_t cols) 
    : m_rows(rows), m_cols(cols) {
    Create();
}

template <typename T>
istream &Matrix1<T>::Read(istream &is) {
    Destroy();
    is >> m_rows >> m_cols;
    Create();
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            is >> m_pMat[i][j];
    return is;
}
// 
template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAllMatriz(Func func, Args&&... args) {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            func(m_pMat[i][j], i, j, forward<Args>(args)...);
        }
    }
}


template <typename T>
ostream &Matrix1<T>::Print(ostream &os) {
    os << m_rows << " " << m_cols << "\n";
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j)
            os << m_pMat[i][j] << " ";
        os << "\n";
    }
    return os;
}

template <typename T>
void Matrix1<T>::Destroy() {
    if (m_pMat != nullptr) {
        for (size_t i = 0; i < m_rows; ++i)
            delete[] m_pMat[i];
        delete[] m_pMat;
        m_pMat = nullptr;
    }
    m_rows = m_cols = 0;
}

// Operadores sobrecargados
template <typename T>
istream &operator>>(istream &is, Matrix1<T> &mat) {
    return mat.Read(is);
}

template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &mat) {
   return mat.Print(os);
}

// Move constructor
template <typename T> 
Matrix1<T>::Matrix1(Matrix1 &&other) {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

//Copy Constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &other){
    m_rows = other.m_rows; 
    m_cols = other.m_cols; 
    Create(); 
    for (size_t i = 0; i < m_rows; ++i) 
        for (size_t j = 0; j < m_cols; ++j) 
            m_pMat[i][j] = other.m_pMat[i][j]; 
}

template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1 &other) const
{
    
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        cout << "Matrix no pueden sumarse debido a dimensiones incompatibles." << endl;
    }

    Matrix1<T> suma(m_rows, m_cols); 
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            suma.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
        }
    }
    return suma;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const
{

    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        cout << "Matrix no pueden sumarse debido a dimensiones incompatibles." << endl;
    }
    Matrix1<T> resta(m_rows, m_cols); 
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            resta.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
        }
    }

    return resta;
}

template <typename T>
Matrix1<T> Matrix1<T>::operatorX(const Matrix1 &other) const {
    if (m_cols != other.m_rows) {
        cout << "Dimensiones incompatibles para multiplicación." << endl;
        return Matrix1<T>(0, 0);
    }
    Matrix1<T> result(m_rows, other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            T sum = 0;
            for (size_t k = 0; k < m_cols; ++k) {
                sum += m_pMat[i][k] * other.m_pMat[k][j];
            }
            result.m_pMat[i][j] = sum;
        }
    }
    return result;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) const
{
    Matrix1<T> result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            result.m_pMat[i][j] = m_pMat[i][j] * value;
        }
    }

    return result;
}


#endif // __MATRIX_H__