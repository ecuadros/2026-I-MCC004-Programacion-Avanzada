#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <functional>
#include <cassert>
#include <cstddef>
#include <utility>
#include <stdexcept>


using namespace std;


template <typename T>
void Print4(T &n, ostream &os) { os << n << " "; }


template <typename T>
class Matrix1 {
    private:
        T **m_pMat=nullptr;
        size_t m_rows=0,m_cols=0;
    public:
        Matrix1(){ }
        ~Matrix1(){ Destroy(); }
        Matrix1(Matrix1 &&other) noexcept;
        Matrix1(const Matrix1 &other);
        Matrix1<T> &operator=(const Matrix1<T> &other);
        Matrix1<T> &operator=(Matrix1<T> &&other) noexcept;
        Matrix1<T> operator+(const Matrix1<T> &other) const;
        Matrix1<T> operator-(const Matrix1<T> &other) const;
        Matrix1<T> operator*(const Matrix1<T> &other) const;
        Matrix1<T> operator*(T value) const;


        void Create();
        istream &Read(istream &is);
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&& ...args);
        ostream &Print(ostream &os);
        void Destroy();
};


template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows>0 && m_cols > 0);
    m_pMat = new T*[m_rows];
    for(size_t i =
        0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}


template <typename T>
Matrix1<T>::Matrix1(Matrix1<T> &&other) noexcept
{
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}


template <typename T>
Matrix1<T>::Matrix1(const Matrix1<T> &other)
{
    m_rows = other.m_rows;
    m_cols = other.m_cols;


    if(other.m_pMat != nullptr)
    {
        Create();
        for(size_t i = 0 ; i < m_rows ; ++i)
            for(size_t j = 0 ; j < m_cols ; ++j)
                m_pMat[i][j] = other.m_pMat[i][j];
    }
}


template <typename T>
Matrix1<T> &Matrix1<T>::operator=(const Matrix1<T> &other)
{
    if(this != &other)
    {
        Destroy();


        m_rows = other.m_rows;
        m_cols = other.m_cols;


        if(other.m_pMat != nullptr)
        {
            Create();


            for(size_t i = 0 ; i < m_rows ; ++i)
                for(size_t j = 0 ; j < m_cols ; ++j)
                    m_pMat[i][j] = other.m_pMat[i][j];
        }
    }


    return *this;
}


template <typename T>
Matrix1<T> &Matrix1<T>::operator=(Matrix1<T> &&other) noexcept
{
    if(this != &other)
    {   Destroy();
        m_pMat = exchange(other.m_pMat, nullptr);
        m_rows = exchange(other.m_rows, 0);
        m_cols = exchange(other.m_cols, 0);
    }


    return *this;
}




//operator +
template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1<T> &other) const
{
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw invalid_argument("Dimensiones incompatibles");


    Matrix1<T> result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();


    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];


    return result;
}


//operator-
template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1<T> &other) const
{
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw invalid_argument("Dimensiones incompatibles");


    Matrix1<T> result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();


    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];


    return result;
}


//muptiplicación operador
template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) const
{
    Matrix1<T> result;


    result.m_rows = m_rows;
    result.m_cols = m_cols;


    result.Create();


    for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] * value;


    return result;
}


template <typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1<T> &other) const
{
    if(m_cols != other.m_rows)
        throw invalid_argument("Dimensiones incompatibles para multiplicacion");


    Matrix1<T> result;


    result.m_rows = m_rows;
    result.m_cols = other.m_cols;


    result.Create();


    for(size_t i = 0 ; i < result.m_rows ; ++i)
    {
        for(size_t j = 0 ; j < result.m_cols ; ++j)
        {
            result.m_pMat[i][j] = T();


            for(size_t k = 0 ; k < m_cols ; ++k)
                result.m_pMat[i][j] += m_pMat[i][k] * other.m_pMat[k][j];
        }
    }


    return result;
}
// parte para completar
template <typename T>
istream &Matrix1<T>::Read(istream &is)
{ Destroy();
    is >> m_rows >> m_cols;
    Create();
    {for(size_t i = 0 ; i < m_rows ; ++i)
        for(size_t j = 0 ; j < m_cols ; ++j)
        is >> m_pMat[i][j];
    }
    return is;
}


template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args)
{for(size_t i = 0 ; i < m_rows ; ++i)
    {for(size_t j = 0 ; j < m_cols ; ++j)
        {func(m_pMat[i][j], args...);}
    }
}


template <typename T>
ostream &Matrix1<T>::Print(ostream &os)
{
    for(size_t i = 0 ; i < m_rows ; ++i)
    {
        for(size_t j = 0 ; j < m_cols ; ++j)
        {os << m_pMat[i][j] << " ";}
        os << endl;
    }


    return os;
}


template <typename T>
void Matrix1<T>::Destroy()
{if(m_pMat != nullptr)
    {for(size_t i=0;i<m_rows;++i)
        {delete[] m_pMat[i];}
        delete[] m_pMat;
        m_pMat = nullptr;
    }
    m_rows = 0;
    m_cols = 0;
}


template <typename T>
istream &operator>>(istream &is, Matrix1<T> &mat)
{return mat.Read(is);}


template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &mat)
{return mat.Print(os);}


template <typename T>
Matrix1<T> operator*(T value, const Matrix1<T> &matrix)
{
    return matrix * value;
}


#endif // __MATRIX_H__
