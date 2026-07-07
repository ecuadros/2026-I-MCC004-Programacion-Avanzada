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
    inline ssize_t Check(ssize_t c){
        if(c < 0) c += cols;
        if(c < 0 || static_cast<size_t>(c) >= cols)
            throw py::index_error("Column index out of bounds");
        return c;
    }
    T Get(ssize_t col){
        col = Check(col);
        return row_ptr[col];
    }
    void Set(ssize_t col, T value){
        col = Check(col);
        row_ptr[col] =  value;
    }
};

template <typename T>
void declare_matrix(py::module &m, const std::string &type_name) {
    py::class_<RowProxy<T>>(m, ("_RowProxy" + type_name).c_str())
        .def("__getitem__", &RowProxy<T>::Get)  // Reading values via the second brackets: value = row[col]
        .def("__setitem__", &RowProxy<T>::Set); // Writing values via the second brackets: row[col] = value

    py::class_<Matrix1<T>> matrix_class(m, ("Matrix1" + type_name).c_str());

    matrix_class
        .def(py::init<size_t, size_t>(), py::arg("rows"), py::arg("cols"))

        // --- NUEVO CONSTRUCTOR DESDE LISTAS DE PYTHON ---
        .def(py::init([](py::list list_of_lists) {
            size_t rows = list_of_lists.size();
            if (rows == 0) {
                return std::make_unique<Matrix1<T>>(0, 0);
            }

            if (!py::isinstance<py::list>(list_of_lists[0])) {
                throw py::type_error("Se esperaba una lista de listas (matriz 2D)");
            }
            size_t cols = list_of_lists[0].cast<py::list>().size();

            auto mat = std::make_unique<Matrix1<T>>(rows, cols);

            for (size_t r = 0; r < rows; ++r) {
                if (!py::isinstance<py::list>(list_of_lists[r])) {
                    throw py::type_error("Todos los elementos de las filas deben ser listas");
                }

                py::list row_list = list_of_lists[r].cast<py::list>();
                if (row_list.size() != cols) {
                    throw py::value_error("Todas las filas deben tener la misma cantidad de columnas");
                }

                for (size_t c = 0; c < cols; ++c) {
                    (*mat)[r][c] = row_list[c].cast<T>();
                }
            }
            return mat;
        }))

        .def_property_readonly("shape", [](const Matrix1<T> &self) {
            return std::make_tuple(self.Rows(), self.Cols());
        })

        .def(py::self + py::self) // Matrix + Matrix
        .def(py::self - py::self) // Matrix - Matrix
        .def(py::self * py::self) // Matrix * Matrix
        .def(py::self * T())      // Matrix * Scalar (value)

        .def("__getitem__", [](Matrix1<T> &self, ssize_t row) {  // Reading values via the first brackets
            if(row < 0) row += self.Rows();
            if(row < 0 || static_cast<size_t>(row) >= self.Rows())
                throw py::index_error("Row index out of bounds");
            return RowProxy<T>(self[row], self.Cols());
        })

        .def("__str__", [](Matrix1<T> &self) {
            std::ostringstream oss;
            oss << self;
            return oss.str();
        })
        .def("apply", [](const Matrix1<T> &self, py::function func) {
            Matrix1<T> result(self.Rows(), self.Cols());

            for (size_t r = 0; r < self.Rows(); ++r) {
                for (size_t c = 0; c < self.Cols(); ++c) {
                    // 1. Extrae el valor de C++
                    T value = self[r][c];

                    // 2. Lo envía a la función de Python y recupera el resultado transformado
                    py::object py_res = func(value);

                    // 3. Convierte de vuelta al tipo T de C++ y lo asigna
                    result[r][c] = py_res.cast<T>();
                }
            }
            return result;
        }, py::arg("func"), "Aplica una función de Python a cada elemento de la matriz");

        py::implicitly_convertible<py::list, Matrix1<T>>();
}

PYBIND11_MODULE(matrix_module, m) {
    m.doc() = "Python bindings for the C++ templated Matrix1 class";

    declare_matrix<double>(m, "Double");
    declare_matrix<int>(m, "Int");
}
