#ifndef __MATRIX_H__
#define __MATRIX_H__
//#include <__nullptr>
#include <functional>
#include <iostream>
#include <cassert>

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
        ~Matrix1()     { Destroy(); }
        void     Create();
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
void Matrix1<T>::Destroy(){
    for(size_t i = 0; i < m_rows; ++i){
        if(m_pMat[i] != nullptr){
            delete[] m_pMat[i];
            m_pMat[i] = nullptr;
        }
    }
}

template <typename T>
istream &Matrix1<T>::Read(istream &is){
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

#endif // __MATRIX_H__