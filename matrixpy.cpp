#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <sstream>
#include <string>
#include "matrix1.h"

/**
 * @file matrixpy.cpp
 * @brief Enlace entre la clase C++ Matrix1 y Python usando pybind11.
 *
 * Este archivo expone Matrix1<int> como una clase Python llamada Matrix dentro
 * del modulo matrix1_cpp. Tambien adapta operadores de C++ a operadores de
 * Python, por ejemplo __mul__, __add__ y __rmul__.
 */

namespace py = pybind11;

/** @brief Alias para exponer una matriz de enteros a Python. */
using MatrixInt = Matrix1<int>;

/**
 * @class MatrixRowInt
 * @brief Representa una fila de la matriz para soportar la sintaxis m[i][j].
 *
 * Python evalua m[i][j] en dos pasos. Primero llama a __getitem__ sobre la
 * matriz y obtiene una fila. Luego llama a __getitem__ o __setitem__ sobre esa
 * fila. Esta clase cumple ese rol intermedio.
 */
class MatrixRowInt {
    private:
        /** @brief Matriz C++ asociada a esta fila. */
        MatrixInt *m_matrix = nullptr;
        /** @brief Indice de fila que representa este objeto. */
        size_t     m_row    = 0;

    public:
        /**
         * @brief Construye una vista de fila para una matriz.
         * @param matrix Matriz original.
         * @param row Indice de fila.
         */
        MatrixRowInt(MatrixInt &matrix, size_t row)
            : m_matrix(&matrix), m_row(row) { }

        /**
         * @brief Obtiene el valor de una columna dentro de la fila.
         * @param col Indice de columna.
         * @return Valor almacenado en la celda.
         */
        int Get(size_t col) const {
            return m_matrix->At(m_row, col);
        }

        /**
         * @brief Asigna un valor a una columna dentro de la fila.
         * @param col Indice de columna.
         * @param value Valor a guardar.
         */
        void Set(size_t col, int value) {
            m_matrix->At(m_row, col) = value;
        }
};

/**
 * @brief Convierte una matriz C++ a texto para print() en Python.
 *
 * Reutiliza el metodo Print de Matrix1 y devuelve el texto como std::string.
 *
 * @param matrix Matriz a convertir.
 * @return Representacion textual de la matriz.
 */
static std::string MatrixToString(const MatrixInt &matrix) {
    std::ostringstream os;
    MatrixInt &mutable_matrix = const_cast<MatrixInt &>(matrix);
    mutable_matrix.Print(os);
    return os.str();
}

/**
 * @brief Define el modulo Python matrix1_cpp.
 *
 * Este bloque registra las clases MatrixRow y Matrix en Python. Tambien enlaza
 * constructores, propiedades, indexacion, impresion y operadores aritmeticos.
 */
PYBIND11_MODULE(matrix1_cpp, m) {
    m.doc() = "Python bindings for the C++ Matrix1<int> class.";

    /** Enlace de la clase auxiliar que representa una fila. */
    py::class_<MatrixRowInt>(m, "MatrixRow")
        .def("__getitem__", &MatrixRowInt::Get)
        .def("__setitem__", &MatrixRowInt::Set);

    /** Enlace de Matrix1<int> como clase Python Matrix. */
    py::class_<MatrixInt>(m, "Matrix")
        .def(py::init<size_t, size_t, int>(), py::arg("rows"), py::arg("cols"), py::arg("value") = 0)
        .def_property_readonly("rows", &MatrixInt::Rows)
        .def_property_readonly("cols", &MatrixInt::Cols)
        .def("__getitem__", [](MatrixInt &matrix, size_t row) {
            return MatrixRowInt(matrix, row);
        })
        .def("__setitem__", [](MatrixInt &matrix, std::pair<size_t, size_t> index, int value) {
            matrix.At(index.first, index.second) = value;
        })
        .def("__str__", &MatrixToString)
        .def("__repr__", &MatrixToString)
        .def("__add__", [](const MatrixInt &lhs, const MatrixInt &rhs) {
            return lhs + rhs;
        })
        .def("__sub__", [](const MatrixInt &lhs, const MatrixInt &rhs) {
            return lhs - rhs;
        })
        .def("__mul__", [](const MatrixInt &lhs, const MatrixInt &rhs) {
            return lhs * rhs;
        })
        .def("__mul__", [](const MatrixInt &lhs, int value) {
            return lhs * value;
        })
        .def("__rmul__", [](const MatrixInt &rhs, int value) {
            return value * rhs;
        })
        .def("transpose", &MatrixInt::Transpose,
             "Devuelve la matriz transpuesta.")
        .def("trace", &MatrixInt::Trace,
             "Calcula la traza de una matriz cuadrada.")
        .def("apply", [](const MatrixInt &matrix, py::function func) {
            MatrixInt result(matrix.Rows(), matrix.Cols());
            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    result.At(i, j) = func(matrix.At(i, j)).cast<int>();
                }
            }
            return result;
        }, "Aplica una funcion Python a cada elemento y devuelve una nueva matriz.")
        .def("extra_apply_cpp", [](const MatrixInt &matrix, int value) {
            return matrix.Apply([value](const int &current) {
                return current + value;
            });
        }, "Aplica un incremento usando el metodo Apply implementado en C++.");
        
}
