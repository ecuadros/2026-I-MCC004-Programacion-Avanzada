// ---------------------------------------------------------------------
//  py_matrix.cpp  -  Exposicion de Matrix1<T> a Python con pybind11
//
//  Se exponen DOS tipos (segun pidio el profesor):
//      matrix1.Matrix1Int    -> Matrix1<int>
//      matrix1.Matrix1Float  -> Matrix1<float>
//
//  Compilar:   python setup.py build_ext --inplace
//  Genera:     matrix1.<plataforma>.so  (o .pyd en Windows)
// ---------------------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
 
#include <sstream>
#include <string>
#include <utility>
 
#include "matrix1.h"
 
namespace py = pybind11;
 
using ssz = py::ssize_t;
 
// ---------------------------------------------------------------------
// Registra la clase Matrix1<T> en el modulo con el nombre "Matrix1" + suffix.
// Como pybind11 no entiende plantillas, se llama una vez por cada tipo
// concreto que queremos exponer (int, float). Todo el binding vive aqui.
// ---------------------------------------------------------------------
template <typename T>
void declare_matrix(py::module &m, const std::string &suffix) {
    using Mat = Matrix1<T>;
    const std::string class_name = "Matrix1" + suffix;
 
    py::class_<Mat>(m, class_name.c_str(), py::buffer_protocol())
        // ---- Constructores ----
        .def(py::init<>())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const T &>())
 
        // ---- Dimensiones ----
        .def("rows", &Mat::Rows)
        .def("cols", &Mat::Cols)
        .def("__len__", &Mat::Rows)
 
        // ---- Fabricas estaticas ----
        .def_static("zeros", &Mat::Zeros)
        .def_static("ones",  &Mat::Ones)
 
        // ---- m[i] : vista de fila ESCRIBIBLE (memoryview) ----
        //   m[i] devuelve una memoryview que apunta a la memoria real de
        //   la fila i, por eso  m[i][j] = v  escribe sobre la matriz.
        .def("__getitem__", [](Mat &mat, size_t row) {
            if (row >= mat.Rows()) throw py::index_error();
            return py::memoryview::from_buffer(
                mat[row],                            // T* no-const => escribible
                { static_cast<ssz>(mat.Cols()) },    // shape
                { static_cast<ssz>(sizeof(T)) }      // stride
            );
        })
 
        // ---- m[i, j] : lectura ----
        .def("__getitem__", [](Mat &mat, std::pair<size_t, size_t> idx) {
            auto [i, j] = idx;
            if (i >= mat.Rows() || j >= mat.Cols()) throw py::index_error();
            return mat[i][j];
        })
 
        // ---- m[i, j] = v : escritura ----
        .def("__setitem__", [](Mat &mat, std::pair<size_t, size_t> idx, T value) {
            auto [i, j] = idx;
            if (i >= mat.Rows() || j >= mat.Cols()) throw py::index_error();
            mat[i][j] = value;
        })
 
        // ---- Protocolo buffer: np.asarray(m) / np.frombuffer(m) ----
        .def_buffer([](Mat &mat) {
            return py::buffer_info(
                mat.Data(),
                sizeof(T),
                py::format_descriptor<T>::format(),
                2,                                                   // dimensiones
                { static_cast<ssz>(mat.Rows()), static_cast<ssz>(mat.Cols()) },
                { static_cast<ssz>(sizeof(T) * mat.Cols()), static_cast<ssz>(sizeof(T)) }
            );
        })
 
        // ---- Comparacion ----
        .def(py::self == py::self)
        .def(py::self != py::self)
 
        // ---- Representacion: print(m) ----
        .def("__repr__", [](const Mat &mat) { std::ostringstream os; os << mat; return os.str(); })
        .def("__str__",  [](const Mat &mat) { std::ostringstream os; os << mat; return os.str(); })
 
        // ---- Operadores matriz-matriz ----
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
 
        // ---- Operadores matriz-escalar ----
        .def(py::self + T())
        .def(py::self - T())
        .def(py::self * T())
        .def(T() + py::self)
        .def(T() - py::self)
        .def(T() * py::self)
 
        // ---- Multiplicacion elemento a elemento ----
        .def("element_wise_multiply", &Mat::ElementWiseMultiply)
 
        // ---- Multiplicacion concurrente (std::thread) ----
        .def("multiply_concurrent", &Mat::MultiplyConcurrent,
             py::arg("other"), py::arg("nthreads") = 0)
 
        // ---- Extras implementados en C++ (matrix1.h) ----
        .def_static("identity", &Mat::Identity)   // Extra 1
        .def("transpose", &Mat::Transpose)        // Extra 2
        .def("trace", &Mat::Trace);               // Extra 3
}
 
PYBIND11_MODULE(matrix1, m) {
    m.doc() = "Matrix1<T> expuesta a Python (pybind11): Matrix1Int y Matrix1Float";
 
    // Una llamada por tipo. Cada una registra una clase Python distinta.
    declare_matrix<int>(m,   "Int");     // -> matrix1.Matrix1Int
    declare_matrix<float>(m, "Float");   // -> matrix1.Matrix1Float
}
 





