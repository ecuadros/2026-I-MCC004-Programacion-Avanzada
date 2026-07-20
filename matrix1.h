#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <functional>
#include <iostream>
#include <cassert>
#include <utility>

using namespace std;

template <typename T>
void Print4(T &n, ostream &os) { os << n << " "; }

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0, m_cols = 0;
    public:
        Matrix1()      { }
        Matrix1(Matrix1 &other) = delete; // No copy constructor
        Matrix1(Matrix1 &&other); 
        ~Matrix1()     { Destroy(); }
        void     Create();
        istream &Read(istream &is);
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&& ...args);
        ostream &Print(ostream &os);
        void Destroy();
        void Init(size_t rows, size_t cols) { Destroy(); m_rows = rows; m_cols = cols; Create(); }
        T *operator[](size_t i)             { return m_pMat[i]; }
        const T *operator[](size_t i) const { return m_pMat[i]; }
        size_t rows() const { return m_rows; }
        size_t cols() const { return m_cols; }
};

template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

// Move constructor
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &&other) {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

template <typename T>
istream &Matrix1<T>::Read(istream &is)
{   is >> m_rows >> m_cols;
    Create();
    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            is >> m_pMat[i][j];
    return is;
}

template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args)
{   for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            func(m_pMat[i][j], std::forward<Args>(args)...);
}

template <typename T>
ostream &Matrix1<T>::Print(ostream &os)
{   os << m_rows << " " << m_cols << "\n";
    for(size_t i = 0 ; i < m_rows ; ++i)
    {   for(size_t j = 0 ; j < m_cols ; ++j)
            os << m_pMat[i][j] << " ";
        os << "\n";
    }
    return os;
}

template <typename T>
void Matrix1<T>::Destroy()
{   if( m_pMat )
    {   for(size_t i = 0 ; i < m_rows ; ++i)
            delete[] m_pMat[i];
        delete[] m_pMat;
        m_pMat = nullptr;
        m_rows = m_cols = 0;
    }
}

template <typename T>
istream &operator>>(istream &is, Matrix1<T> &mat) { return mat.Read(is); }

template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &mat) { return mat.Print(os); }

#endif // __MATRIX_H__