/**
 * @file C++ToPython.cpp
 * @brief Enlace de la clase Matrix1 a Python mediante pybind11.
 * @details Define el módulo 'MatrizToString' que expone la clase Matrix1<B1>.
 */
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream> 
#include <iostream>
#include "matrix1.h"
#include "types.h"

using namespace std;
namespace py = pybind11;

/**
 * @brief Módulo para Python que encapsula Matrix1.
 */
PYBIND11_MODULE(MatrizToString,m){
    m.doc() = "Enlace para Python";
    py::class_<Matrix1<B1>>(m, "Matrix1")

    .def(py::init<>())
    .def(py::init<size_t, size_t>())

    .def("__repr__", [](const Matrix1<B1>& self) {return MatrizToString(self); })
    .def("__str__", [] (const Matrix1<B1>& self) {return MatrizToString(self); })

    .def("__add__", &Matrix1<B1>::operator+, py::is_operator())
    .def("__sub__", &Matrix1<B1>::operator-, py::is_operator())
    .def("__mul__", [](const Matrix1<B1>& self, B1 val) {return self * val; }, py::is_operator())
    .def("__mul__", [](const Matrix1<B1>& self, const Matrix1<B1>& other) {return self * other;}, py::is_operator()) 
    .def("__rmul__", [](const Matrix1<B1>& self, B1 val) { return self * val; }, py::is_operator())
    .def("__truediv__", &Matrix1<B1>::operator/, py::is_operator())
    .def("det", &Matrix1<B1>::operatorDet)


    .def("__call__", [](Matrix1<B1>& self, size_t row, size_t col) -> B1& {
        return self(row, col); }, py::return_value_policy::reference)
    .def("__call__", [](const Matrix1<B1>& self, size_t row, size_t col) -> const B1& {
        return self(row, col); })
    .def("set", [](Matrix1<B1>& self, size_t row, size_t col, B1 val) {
        self(row, col) = val; })
    .def_property_readonly("rows", &Matrix1<B1>::getRows)
    .def_property_readonly("cols", &Matrix1<B1>::getCols);


}