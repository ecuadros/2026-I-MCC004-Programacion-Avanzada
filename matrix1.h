#ifndef __MATRIX_H__
#define __MATRIX_H__
// #include <__nullptr>
#include <functional>
#include <iostream>
#include <cassert>

using namespace std;

// template <typename T>
// void Print4(T &n, ostream &os) { os << n << " "; }

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0, m_cols = 0;
        template <typename Func>
        void ForEachIndex(Func func) const;
    public:
        Matrix1()      { }
        ~Matrix1()     { Destroy(); }
        Matrix1(Matrix1 &&other) noexcept; // Move constructor
        Matrix1(const Matrix1 &other); // Copy Constructor
 
        Matrix1<T> &operator=(const Matrix1<T> &other); // Operator = - Copy assignment operator
        Matrix1<T> &operator=(Matrix1<T> &&other) noexcept; // Operator = - Move assignment operator

        Matrix1<T> operator+(const Matrix1<T> &other) const; // Operator +
        Matrix1<T> operator-(const Matrix1<T> &other) const; // Operator -
        Matrix1<T> operator*(const Matrix1<T> &other) const; // Operator *
        Matrix1<T> operator*(T value) const; // Operator * escalar

        void Create();
        istream &Read(istream &is);
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&& ...args);
        ostream &Print(ostream &os);
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
template <typename Func>
void Matrix1<T>::ForEachIndex(Func func) const {
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            func(i, j);
}

// Copy Constructor
template <typename T>
Matrix1<T>::Matrix1(const Matrix1 &other) {
    m_rows = other.m_rows;
    m_cols = other.m_cols;

    if(other.m_pMat != nullptr) {
    Create();

    ForEachIndex([&](size_t i, size_t j) {
        m_pMat[i][j] = other.m_pMat[i][j];
    });
    }
}

// Move constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &&other) noexcept {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

template <typename T>
istream &Matrix1<T>::Read(istream &is) {
    Destroy();
    is >> m_rows >> m_cols;
    Create();
    ForEachIndex([&](size_t i, size_t j) {
        is >> m_pMat[i][j];
    });
    return is;
}

template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args) {
    ForEachIndex([&](size_t i, size_t j) {
        func(m_pMat[i][j], std::forward<Args>(args)...);
    });
}

template <typename T>
ostream &Matrix1<T>::Print(ostream &os) {
    os << "Dimensiones: ";
    os << m_rows << " x " << m_cols << "\n";
    ForEachIndex([&](size_t i, size_t j) {
        os << m_pMat[i][j] << " ";
        if (j + 1 == m_cols)
            os << endl;
    });
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
    m_rows = 0;
    m_cols = 0;
}

template <typename T>
istream &operator>>(istream &is, Matrix1<T> &matrix) {
    return matrix.Read(is);
}

template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &matrix) {
    return matrix.Print(os);
}

// Copy assignment operator
template <typename T>
Matrix1<T> &Matrix1<T>::operator=(const Matrix1<T> &other) {
    if (this != &other) {
        Destroy();

        m_rows = other.m_rows;
        m_cols = other.m_cols;
       
        if(other.m_pMat != nullptr) {
            Create();

            ForEachIndex([&](size_t i, size_t j) {
                m_pMat[i][j] = other.m_pMat[i][j];
            });
        }
    }
    return *this;
}

// Move assignment operator
template <typename T>
Matrix1<T> &Matrix1<T>::operator=(Matrix1<T> &&other) noexcept {
    if (this != &other) {
        Destroy();

        m_pMat = exchange(other.m_pMat, nullptr);
        m_rows = exchange(other.m_rows, 0);
        m_cols = exchange(other.m_cols, 0);
    }
    return *this;
}

// Operator +
template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1<T> &other) const {
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Matrices must have the same dimensions for addition.");
    
    Matrix1<T> result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
    });

    return result;
}

// Operator -
template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const {
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");

    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
    });
    return result;
}

// Operator(const Matrix1<T> &other)* -- Multiplica dos matrices
template <typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1 &other) const {
    if(m_cols != other.m_rows)
        throw std::invalid_argument("Columnas de la primera matriz deben ser iguales a filas de la segunda matriz para multiplicación.");

    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = other.m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = T(); // Initialize to zero
        for (size_t k = 0; k < m_cols; ++k)
            result.m_pMat[i][j] += m_pMat[i][k] * other.m_pMat[k][j];
    });
    return result;
}

// Operator(T value)* -- Multiplica cada elemento de la matriz por un valor escalar - Operador miembro
template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) const {
    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] * value;
    });

    return result;
}

// Operator(T value, const Matrix1<T> &matrix) -- Multiplica escalar por matriz cuando el escalar está a la izquierda
template <typename T>
Matrix1<T> operator*(T value, const Matrix1<T> &matrix) {
    return matrix * value; // Reutiliza la implementación de multiplicación escalar
}

#endif // __MATRIX_H__
