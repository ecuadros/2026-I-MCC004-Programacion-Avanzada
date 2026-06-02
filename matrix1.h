#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <functional>
#include <iostream>
#include <utility>  // para std::exchange
#include <cassert>  // para assert

using namespace std;

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0, m_cols = 0;
    public:
        //Matrix1()      { }
        //Matrix1(Matrix1 &other) = delete; // No copy constructor
        Matrix1() : m_pMat(nullptr), m_rows(0), m_cols(0) { } //
        Matrix1(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) { 
            if (rows > 0 && cols > 0) Create(); 
        }
        Matrix1(const Matrix1 &other); // copy constructor
        Matrix1(Matrix1 &&other);       // move constructor
        ~Matrix1()     { Destroy(); }
        
        Matrix1& operator=(const Matrix1 &other);  // copy assignment
        Matrix1& operator=(Matrix1 &&other);       // move assignment       
        
        
        void Create();
        void Destroy();
        
        ostream &Print(ostream &os);
        istream& Read(istream &is);
        
        T& operator()(size_t row, size_t col) { 
            assert(m_pMat != nullptr);
            return m_pMat[row][col]; 
        } //
        
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&&... args);
        
        Matrix1 operator+(const Matrix1 &other);   // suma
        Matrix1 operator-(const Matrix1 &other);   // resta
        Matrix1 operator*(const Matrix1 &other);   // multiplicación
        Matrix1 operator*(T value);                // multiplicación por escalar
};

template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

// Copy constructor 
template <typename T>
Matrix1<T>::Matrix1(const Matrix1 &other) {
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    if (other.m_pMat != nullptr && m_rows > 0 && m_cols > 0) {
        Create();   // reserva m_pMat con m_rows x m_cols
        for (size_t i = 0; i < m_rows; ++i)
            for (size_t j = 0; j < m_cols; ++j)
                m_pMat[i][j] = other.m_pMat[i][j];
    } else {
        m_pMat = nullptr;
    }
}

// Move constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &&other) {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
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

// Copy assignment operator
template <typename T>
Matrix1<T>& Matrix1<T>::operator=(const Matrix1<T> &other) {
    if (this != &other) {
        Destroy();
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        if (other.m_pMat != nullptr && m_rows > 0 && m_cols > 0) {
            Create();
            for (size_t i = 0; i < m_rows; ++i)
                for (size_t j = 0; j < m_cols; ++j)
                    m_pMat[i][j] = other.m_pMat[i][j];
        } else {
            m_pMat = nullptr;
        }
    }
    return *this;
}

// Move assignment operator
template <typename T>
Matrix1<T>& Matrix1<T>::operator=(Matrix1<T> &&other) {
    if (this != &other) {
        Destroy();
        m_pMat = exchange(other.m_pMat, nullptr);
        m_rows = exchange(other.m_rows, 0);
        m_cols = exchange(other.m_cols, 0);
    }
    return *this;
}

template <typename T>
ostream &Matrix1<T>::Print(ostream &os) {
    if (m_pMat == nullptr || m_rows == 0 || m_cols == 0) {
        os << "Matriz vacia" << endl;
        return os;
    }
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j)
            os << m_pMat[i][j] << " ";
        os << endl;
    }
    return os;
}

template <typename T>
istream& Matrix1<T>::Read(istream &is) {
    Destroy();
    is >> m_rows >> m_cols;
    if (m_rows > 0 && m_cols > 0) {
        Create();
        for (size_t i = 0; i < m_rows; ++i)
            for (size_t j = 0; j < m_cols; ++j)
                is >> m_pMat[i][j];
    }
    return is;
}

// Implementación de ApplyFunctionToAll (DESPUÉS de los otros métodos)
template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&&... args) {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            func(m_pMat[i][j], std::forward<Args>(args)...);
        }
    }
}


// operator+ (suma elemento a elemento)
template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1 &other) {
    assert(m_rows == other.m_rows && m_cols == other.m_cols);
    Matrix1 result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
    return result;
}

// operator- (resta elemento a elemento)
template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) {
    assert(m_rows == other.m_rows && m_cols == other.m_cols);
    Matrix1 result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
    return result;
}

// operator* (multiplicación de matrices)
template <typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1 &other) {
    assert(m_cols == other.m_rows);
    Matrix1 result(m_rows, other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            result.m_pMat[i][j] = 0;
            for (size_t k = 0; k < m_cols; ++k)
                result.m_pMat[i][j] += m_pMat[i][k] * other.m_pMat[k][j];
        }
    }
    return result;
}

// operator* (multiplicación por escalar)
template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) {
    Matrix1 result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] * value;
    return result;
}

// Operadores globales
template<typename T>
std::ostream& operator<<(std::ostream& os, Matrix1<T>& matrix) {
    return matrix.Print(os);
}

template<typename T>
std::istream& operator>>(std::istream& is, Matrix1<T>& matrix) {
    return matrix.Read(is);
}

template<typename T>
Matrix1<T> operator*(T scalar, Matrix1<T>& matrix) {
    return matrix * scalar;
}

#endif // __MATRIX_H__