#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream>
#include "types.h"
#include "matrix1.h"

namespace py = pybind11;

template <typename T>
struct RowProxy {
    T* row_ptr;
    size_t cols;

    RowProxy(T* ptr, size_t c) : row_ptr(ptr), cols(c) {}
    T Get(size_t col){
        if (col >= cols) throw py::index_error("Column index out of bounds");
        return row_ptr[col];
    }
    void Set(size_t col, T value){
        if (col >= cols) throw py::index_error("Column index out of bounds");
        row_ptr[col] =  value;
    }
};

template <typename T>
void declare_matrix(py::module &m, const std::string &type_name) {
    py::class_<RowProxy<T>>(m, ("_RowProxy" + type_name).c_str())
        .def("__getitem__", &RowProxy<T>::Get)  // Reading values via the second brackets: value = row[col]
        .def("__setitem__", &RowProxy<T>::Set); // Writing values via the second brackets: row[col] = value

    py::class_<Matrix1<T>>(m, ("Matrix1" + type_name).c_str())
        .def(py::init<size_t, size_t>(), py::arg("rows"), py::arg("cols"))

        .def_property_readonly("shape", [](const Matrix1<T> &self) {
            return std::make_tuple(self.Rows(), self.Cols());
        })

        .def(py::self + py::self) // Matrix + Matrix
        .def(py::self - py::self) // Matrix - Matrix
        .def(py::self * py::self) // Matrix * Matrix
        .def(py::self * T())      // Matrix * Scalar (value)

        .def("__getitem__", [](Matrix1<T> &self, size_t row) {  // Reading values via the first brackets
            if (row >= self.Rows()) throw py::index_error("Row index out of bounds");
            return RowProxy<T>(self[row], self.Cols());
        })

        .def("__str__", [](Matrix1<T> &self) {
            std::ostringstream oss;
            oss << self;
            return oss.str();
        });
}

PYBIND11_MODULE(matrix_module, m) {
    m.doc() = "Python bindings for the C++ templated Matrix1 class";

    declare_matrix<double>(m, "Double");
    declare_matrix<int>(m, "Int");
}
