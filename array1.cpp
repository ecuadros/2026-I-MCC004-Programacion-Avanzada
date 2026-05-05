#include <iostream>
#include "array1.h"

using namespace std;


void CreateArray1(TP *&pArray, size_t n) {
    pArray = new TP[n];
}

void ReadArray1(TP *pArray, size_t n) {
    for (size_t i = 0; i < n; i++) {
        cout << "Ingrese un numero: ";
    cin >> pArray[i];
  }
}

void PrintArray1(TP *pArray, size_t n) {
    for (size_t i = 0; i < n; i++)
        cout << pArray[i] << " ";
    cout << endl;
}

void DeleteArray1(TP *&pArray) {
    if (pArray != nullptr) {
        delete[] pArray;
        pArray = nullptr;
    }
}