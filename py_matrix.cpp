// ---------------------------------------------------------------------
//  py_matrix.cpp  -  Exposicion de Matrix1<double> a Python con pybind11
//
//  Compilar:   python setup.py build_ext --inplace
//  Genera:     matrix1.<plataforma>.so  (o .pyd en Windows)
// ---------------------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
 
#include <sstream>
#include <utility>
 
#include "matrix1.h"
 
namespace py = pybind11;
 
using TD   = double;          // tipo de los elementos expuesto a Python
using Mat  = Matrix1<TD>;
using ssz  = py::ssize_t;
 
PYBIND11_MODULE(matrix1, m) {
    m.doc() = "Matrix1<double> expuesta a Python (pybind11)";
 
    py::class_<Mat>(m, "Matrix1", py::buffer_protocol())
        // ---- Constructores ----
        .def(py::init<>())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const TD &>())
 
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
                mat[row],                            // TD* no-const => escribible
                { static_cast<ssz>(mat.Cols()) },    // shape
                { static_cast<ssz>(sizeof(TD)) }     // stride
            );
        })
 
        // ---- m[i, j] : lectura ----
        .def("__getitem__", [](Mat &mat, std::pair<size_t, size_t> idx) {
            auto [i, j] = idx;
            if (i >= mat.Rows() || j >= mat.Cols()) throw py::index_error();
            return mat[i][j];
        })
 
        // ---- m[i, j] = v : escritura ----
        .def("__setitem__", [](Mat &mat, std::pair<size_t, size_t> idx, TD value) {
            auto [i, j] = idx;
            if (i >= mat.Rows() || j >= mat.Cols()) throw py::index_error();
            mat[i][j] = value;
        })
 
        // ---- Protocolo buffer: np.asarray(m) / np.frombuffer(m) ----
        .def_buffer([](Mat &mat) {
            return py::buffer_info(
                mat.Data(),
                sizeof(TD),
                py::format_descriptor<TD>::format(),
                2,                                                   // dimensiones
                { static_cast<ssz>(mat.Rows()), static_cast<ssz>(mat.Cols()) },
                { static_cast<ssz>(sizeof(TD) * mat.Cols()), static_cast<ssz>(sizeof(TD)) }
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
        .def(py::self + TD())
        .def(py::self - TD())
        .def(py::self * TD())
        .def(TD() + py::self)
        .def(TD() - py::self)
        .def(TD() * py::self)
 
        // ---- Multiplicacion elemento a elemento ----
        .def("element_wise_multiply", &Mat::ElementWiseMultiply)
 
        // ---- Multiplicacion concurrente (std::thread) ----
        .def("multiply_concurrent", &Mat::MultiplyConcurrent,
             py::arg("other"), py::arg("nthreads") = 0)
 
        // ---- Extras implementados en C++ (matrix1.h) ----
        .def_static("identity", &Mat::Identity)   // Extra 1
        .def("transpose", &Mat::Transpose)        // Extra 2
        .def("trace", &Mat::Trace);               // Extra 3
 
    // Alias a nivel de modulo:  matrix1.zeros(...) / matrix1.ones(...) / matrix1.identity(...)
    m.def("zeros",    &Mat::Zeros);
    m.def("ones",     &Mat::Ones);
    m.def("identity", &Mat::Identity);
}