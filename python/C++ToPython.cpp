/**
 * @file C++ToPython.cpp
 * @brief Enlace de la clase Matrix1 a Python mediante pybind11.
 * @details Define el módulo 'matrix1' que expone las clases Matrix1<ti> y Matrix1<B1> (int y double),
 *          junto con sus respectivos proxies de fila.
 */
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream>
#include <iostream>
#include "matrix1.h"
#include "types.h"

using namespace std;
namespace py = pybind11;

template <typename T>
class RowProxy {
private:
    T* row_ptr;
    size_t cols;
public:
    RowProxy(T* row, size_t ncols) : row_ptr(row), cols(ncols) {}

    T& operator[](size_t col) {
        if (col >= cols) throw py::index_error("Índice de columna fuera de rango");
        return row_ptr[col];
    }

    const T& operator[](size_t col) const {
        if (col >= cols) throw py::index_error("Índice de columna fuera de rango");
        return row_ptr[col];
    }

    size_t size() const { return cols; }
};


template <typename T>
void bind_matrix(py::module &m, const std::string &cls_name, const std::string &row_proxy_name) {
    // 1) Vincular RowProxy<T>
    py::class_<RowProxy<T>>(m, row_proxy_name.c_str())
        .def("__getitem__", [](RowProxy<T>& self, size_t col) -> T& {
            return self[col];
        })
        .def("__setitem__", [](RowProxy<T>& self, size_t col, T val) {
            self[col] = val;
        })
        .def("__len__", &RowProxy<T>::size);

    // 2) Vincular Matrix1<T>
    py::class_<Matrix1<T>>(m, cls_name.c_str())
        .def(py::init<>())
        .def(py::init<size_t, size_t>())

        // Acceso con corchetes: devuelve un RowProxy
        .def("__getitem__", [](Matrix1<T>& self, size_t row) -> RowProxy<T> {
            if (row >= self.getRows()) throw py::index_error("Índice de fila fuera de rango");
            return RowProxy<T>(self[row], self.getCols());
        })
        .def("__len__", &Matrix1<T>::getRows)

        // Representación
        .def("__repr__", [](const Matrix1<T>& self) {
            return MatrizToString(self);
        })
        .def("__str__",  [](const Matrix1<T>& self) {
            return MatrizToString(self);
        })

        .def("__add__", &Matrix1<T>::operator+, py::is_operator())
        .def("__sub__", &Matrix1<T>::operator-, py::is_operator())
        .def("__mul__", [](const Matrix1<T>& self, T val) { return self * val; }, py::is_operator())
        .def("__mul__", [](const Matrix1<T>& self, const Matrix1<T>& other) { return self * other; }, py::is_operator())
        .def("__rmul__", [](const Matrix1<T>& self, T val) { return self * val; }, py::is_operator())
        .def("__truediv__", &Matrix1<T>::operator/, py::is_operator())

        .def("det", &Matrix1<T>::operatorDet)

        // Acceso estilo función: m(i,j)
        .def("__call__", [](Matrix1<T>& self, size_t row, size_t col) -> T& {
            return self(row, col);
        }, py::return_value_policy::reference)
        .def("__call__", [](const Matrix1<T>& self, size_t row, size_t col) -> const T& {
            return self(row, col);
        })
        .def("set", [](Matrix1<T>& self, size_t row, size_t col, T val) {
            self(row, col) = val;
        })

       
        .def_property_readonly("rows", &Matrix1<T>::getRows)
        .def_property_readonly("cols", &Matrix1<T>::getCols);
}


PYBIND11_MODULE(MatrizToString, m) {
    m.doc() = "Matrices de enteros y doubles";
    
    bind_matrix<TI>(m, "Matrix1", "RowProxy1");// Matriz de enteros (TI = int)
    bind_matrix<B1>(m, "MatrixD", "RowProxyD");// Matriz de dobles (B1 = double)
}