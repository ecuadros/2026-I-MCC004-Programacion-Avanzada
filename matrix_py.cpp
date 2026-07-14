#include <pybind11/pybind11.h>
#include <sstream>
#include <string>
#include <stdexcept>
#include "matrix1.h"

namespace py = pybind11;
using namespace std;

template <typename T>
void RegistrarMatrix(py::module_ &m, const char *nombre)
{
    py::class_<Matrix1<T>>(m, nombre)
        .def(py::init<size_t, size_t>())

        .def("set", &Matrix1<T>::Set)
        .def("get", &Matrix1<T>::Get)
        .def("rows", &Matrix1<T>::Rows)
        .def("cols", &Matrix1<T>::Cols)

        .def("__mul__", [](const Matrix1<T> &a, const Matrix1<T> &b) {
            return a * b;
        })

        .def("__str__", [](Matrix1<T> &mat) {
            ostringstream os;
            os << mat;
            return os.str();
        });
}

PYBIND11_MODULE(matrix_py, m)
{
    RegistrarMatrix<int>(m, "MatrixInt");
    RegistrarMatrix<float>(m, "MatrixFloat");

    m.def("crear_matriz", [](const string &tipo, size_t filas, size_t columnas) -> py::object {
        if(tipo == "int")
            return py::cast(Matrix1<int>(filas, columnas));

        if(tipo == "float")
            return py::cast(Matrix1<float>(filas, columnas));

        throw invalid_argument("Tipo no valido. Use int o float.");
    });
}