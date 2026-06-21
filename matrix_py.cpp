#include <pybind11/pybind11.h>
#include <sstream>
#include "matrix1.h"

namespace py = pybind11;

PYBIND11_MODULE(matrix_py, m)
{
    py::class_<Matrix1<int>>(m, "Matrix")
        .def(py::init<size_t, size_t>())

        .def("set", &Matrix1<int>::Set)
        .def("get", &Matrix1<int>::Get)
        .def("rows", &Matrix1<int>::Rows)
        .def("cols", &Matrix1<int>::Cols)

        .def("__mul__", [](const Matrix1<int> &a, const Matrix1<int> &b) {
            return a * b;
        })

        .def("__str__", [](Matrix1<int> &mat) {
            std::ostringstream os;
            os << mat;
            return os.str();
        });
}