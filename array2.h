#ifndef __ARRAY2_H__
#define __ARRAY2_H__
#include <functional>
#include <iostream>

using namespace std;

template <typename T>
void CreateArray2(T *&pArray, size_t n) {
    pArray = new T[n];
}

template <typename T>
void ReadArray2(T *pArray, size_t n, istream &is) {
    for (size_t i = 0; i < n; ++i)
        is >> pArray[i];
}

template <typename T, typename Func>
void ApplyFunctionToAll2(T *pArray, size_t n, Func func) {
    for (size_t i = 0; i < n; ++i)
        func(pArray[i]);
}

template <typename T>
void PrintArray2(T *pArray, size_t n, ostream &os) {
    os << n << " \n";
    for (size_t i = 0; i < n; ++i)
        os << pArray[i] << " ";
    os << endl;
}

template <typename T>
void DeleteArray2(T *&pArray) {
    if (pArray != nullptr) {
        delete[] pArray;
        pArray = nullptr;
    }
}

#endif // __ARRAY2_H__