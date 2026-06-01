#ifndef __MATRIX_H__
#define __MATRIX_H__
//#include <__nullptr>
#include <functional>
#include <iostream>
#include <cassert>
#include <utility>  //exchange

using namespace std;

//template <typename T>
//void Print4(T &n, ostream &os) { os << n << " "; }

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

        // Operator overloading
        Matrix1 operator+(const Matrix1& m);
        Matrix1 operator-(Matrix1& m);
        Matrix1 operator*(Matrix1& m);
        Matrix1 operator*(T value);
        template <typename U>
        friend Matrix1<U> operator*(U value, Matrix1<U>& m);
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
    static TI mov_cnt;
    cout << "Move constructor call #" << ++mov_cnt << endl;
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

template <typename T>
void Matrix1<T>::Destroy(){
    for(size_t i = 0; i < m_rows; ++i){
        if(m_pMat[i] != nullptr){
            delete[] m_pMat[i];
            m_pMat[i] = nullptr;
        }
    }
    if(m_pMat != nullptr){
        delete[] m_pMat;
        m_pMat = nullptr;
    }
    m_rows = 0;
    m_cols = 0;
}

template <typename T>
istream &Matrix1<T>::Read(istream &is){
    Destroy();
    is >> m_rows;
    is >> m_cols;
    Create();
    for(size_t i = 0; i < m_rows; ++i){
        for (size_t j = 0; j < m_cols; ++j){
            is >> m_pMat[i][j];
        }
    }
    return is;
}

template <typename T>
ostream &Matrix1<T>::Print(ostream &os){
    os << m_rows << "\n";
    os << m_cols << "\n";
    for(size_t i = 0; i < m_rows; ++i){
        for(size_t j = 0; j < m_cols; ++j){
            os << m_pMat[i][j] << " ";
        }
        os << "\n";
    }
    os << endl;
    return os;
}

template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args){
    for(size_t i = 0; i < m_rows; ++i){
        for(size_t j = 0; j < m_cols; ++j){
            func(m_pMat[i][j], forward<Args>(args)...);
        }
    }
}

template <typename T>
istream &operator>>(istream &is, Matrix1<T> &matrix){
    return matrix.Read(is);
}

template<typename T>
ostream &operator<<(ostream &os, Matrix1<T> &matrix){
    matrix.ApplyFunctionToAll(Print4<T>, os);
    return os;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1<T>& m){
    assert(this->m_rows == m.m_rows && this->m_cols == m.m_cols);
    Matrix1<T> res;
    res.m_rows = m.m_rows;
    res.m_cols = m.m_cols;
    res.Create();

    for(size_t i = 0; i < res.m_rows; ++i){
        for(size_t j = 0; j < res.m_cols; ++j){
            res.m_pMat[i][j] = this->m_pMat[i][j] + m.m_pMat[i][j];
        }
    }
    return res;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator-(Matrix1<T>& m){
    assert(this->m_rows == m.m_rows && this->m_cols == m.m_cols);
    Matrix1<T> res;
    res.m_rows = m.m_rows;
    res.m_cols = m.m_cols;
    res.Create();

    for(size_t i = 0; i < res.m_rows; ++i){
        for(size_t j = 0; j < res.m_cols; ++j){
            res.m_pMat[i][j] = this->m_pMat[i][j] - m.m_pMat[i][j];
        }
    }
    return res;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator*(Matrix1<T>& m){
    assert(this->m_cols == m.m_rows);
    Matrix1<T> res;
    res.m_rows = this->m_rows;
    res.m_cols = m.m_cols;
    res.Create();

    for(size_t i = 0; i < res.m_rows; ++i){
        for(size_t j = 0; j < res.m_cols; ++j){
            res.m_pMat[i][j] = 0;
            for(size_t k = 0; k < this->m_cols; ++k){
                res.m_pMat[i][j] += this->m_pMat[i][k]*m.m_pMat[k][j];
            }
        }
    }
    return res;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value){
    Matrix1<T> res;
    res.m_rows = this->m_rows;
    res.m_cols = this->m_cols;
    res.Create();

    for(size_t i = 0; i < res.m_rows; ++i){
        for(size_t j = 0; j < res.m_cols; ++j){
            res.m_pMat[i][j] = this->m_pMat[i][j]*value;
        }
    }
    return res;
}

template <typename T>
Matrix1<T> operator*(T value, Matrix1<T>& m){
    Matrix1<T> res;
    res.m_rows = m.m_rows;
    res.m_cols = m.m_cols;
    res.Create();
    for(size_t i = 0; i < res.m_rows; ++i){
        for(size_t j = 0; j < res.m_cols; ++j){
            res.m_pMat[i][j] = m.m_pMat[i][j]*value;
        }
    }
    return res;
}

#endif // __MATRIX_H__