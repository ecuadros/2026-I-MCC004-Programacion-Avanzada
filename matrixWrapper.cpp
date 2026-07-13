#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include "matrix1.h"

namespace py = pybind11;

// Estructura Proxy para soportar la sintaxis m[i][j] en Python
template <typename T>
struct RowProxy {
    T* row_ptr;
    T getitem(size_t c) const { return row_ptr[c]; }
    void setitem(size_t c, T val) { row_ptr[c] = val; }
};

// Función auxiliar para redirigir tu método Print() a un string para Python
template <typename T>
std::string MatrixToString(Matrix1<T>& m) {
    std::stringstream ss;
    m.Print(ss);
    return ss.str();
}

//----- Modulo principal -> pybind11
PYBIND11_MODULE(matrix_lib, m) { // matrix_lib : Nombre de la libreria, m : libreria de c++
    m.doc() = "Librería de matrices en C++ expuesta a Python";

    // Registramos la clase auxiliar ... ojo : por ahora enfocado a tipos int
    py::class_<RowProxy<int>>(m, "_RowProxyInt")
        .def("__getitem__", &RowProxy<int>::getitem)
        .def("__setitem__", &RowProxy<int>::setitem);

    // Registramos clase para tipos int
    py::class_<Matrix1<int>>(m, "MatrixInt")
        .def(py::init<size_t, size_t>())
        .def("create", &Matrix1<int>::Create)
        .def("destroy", &Matrix1<int>::Destroy)
        
        // llamando a las 3 extras implementadas
        .def("diag", &Matrix1<int>::Diag)
        .def("transpose", &Matrix1<int>::Transpose)
        .def("det", &Matrix1<int>::Det)
        
        // Exponemos las operaciones matemáticas que definiste
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self * int())
        
        // Hacemos que print(matriz) en Python muestre la matriz
        .def("__str__", &MatrixToString<int>)

        // SOPORTE PARA SINTAXIS m[i][j] (Devuelve el Proxy)
        .def("__getitem__", [](Matrix1<int> &instance, size_t r) {
            return RowProxy<int>{ instance[r] }; 
        })
        
        // SOPORTE PARA SINTAXIS m[i, j] (Llama directamente a tu operator())
        .def("__getitem__", [](Matrix1<int> &instance, std::pair<size_t, size_t> idx) {
            return instance(idx.first, idx.second);
        })
        .def("__setitem__", [](Matrix1<int> &instance, std::pair<size_t, size_t> idx, int val) {
            instance(idx.first, idx.second) = val;
        });
        
}
