#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "matrix1.h"

using namespace std;
namespace py = pybind11;


template<typename T>
Matrix1<T> crear_matriz(py::list filas) {
    if (filas.empty()) return Matrix1<T>();
    size_t nrows = filas.size();
    py::list primera = filas[0].cast<py::list>();
    size_t ncols = primera.size();
    Matrix1<T> mat(nrows, ncols);
    for (size_t i = 0; i < nrows; ++i) {
        py::list fila = filas[i].cast<py::list>();
        if (fila.size() != ncols)
            throw runtime_error("Todas las filas deben tener el mismo tamaño");
        for (size_t j = 0; j < ncols; ++j) {
            mat(i, j) = fila[j].cast<T>();
        }
    }
    return mat;
}

// Macro para exponer una instancia de Matrix1<T> en Python
#define EXPONER_MATRIZ(T, nombre) \
    py::class_<Matrix1<T>>(m, nombre) \
        .def(py::init<>()) \
        .def(py::init<size_t, size_t>()) \
        .def("__repr__", [](const Matrix1<T>& self) { return MatrizToString(self); }) \
        .def("__str__", [](const Matrix1<T>& self) { return MatrizToString(self); }) \
        .def("__add__", &Matrix1<T>::operator+, py::is_operator()) \
        .def("__sub__", &Matrix1<T>::operator-, py::is_operator()) \
        .def("__mul__", [](const Matrix1<T>& self, T val) { return self * val; }, py::is_operator()) \
        .def("__mul__", [](const Matrix1<T>& self, const Matrix1<T>& other) { return self * other; }, py::is_operator()) \
        .def("__rmul__", [](const Matrix1<T>& self, T val) { return self * val; }, py::is_operator()) \
        .def("__truediv__", &Matrix1<T>::operator/, py::is_operator()) \
        .def("det", &Matrix1<T>::operatorDet) \
        .def("__call__", [](Matrix1<T>& self, size_t row, size_t col) -> T& { return self(row, col); }, py::return_value_policy::reference) \
        .def("__call__", [](const Matrix1<T>& self, size_t row, size_t col) -> const T& { return self(row, col); }) \
        .def("set", [](Matrix1<T>& self, size_t row, size_t col, T val) { self(row, col) = val; }) \
        .def_property_readonly("rows", &Matrix1<T>::getRows) \
        .def_property_readonly("cols", &Matrix1<T>::getCols);

PYBIND11_MODULE(MatrizToString, m) {
    m.doc() = "Matrices con tipos int, long, double";

    // Exponer Matrix1<int>, Matrix1<long>, Matrix1<double>
    EXPONER_MATRIZ(int, "Matrix1-Int")
    EXPONER_MATRIZ(long, "Matrix1-Long")
    EXPONER_MATRIZ(double, "Matrix1-Double")

    // Funciones de fábrica para crear matrices desde listas de Python
    m.def("from_list_int", [](py::list filas) { return crear_matriz<int>(filas); });
    m.def("from_list_long", [](py::list filas) { return crear_matriz<long>(filas); });
    m.def("from_list_double", [](py::list filas) { return crear_matriz<double>(filas); });
}