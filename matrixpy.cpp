#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <future>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include "matrix1.h"
#include "types.h"

/**
 * @file matrixpy.cpp
 * @brief Enlace entre la clase C++ Matrix1 y Python usando pybind11.
 *
 * Este archivo expone dos especializaciones de Matrix1: Matrix1<TI> y
 * Matrix1<T3F>. Tambien adapta operadores de C++ a operadores de Python,
 * por ejemplo __mul__, __add__ y __rmul__.
 */

namespace py = pybind11;

/** @brief Alias para exponer una matriz de enteros a Python. */
using MatrixInt = Matrix1<TI>;

/** @brief Alias para exponer una matriz de flotantes a Python. */
using MatrixFloat = Matrix1<T3F>;

/**
 * @class MatrixRow
 * @brief Representa una fila de la matriz para soportar la sintaxis m[i][j].
 *
 * Python evalua m[i][j] en dos pasos. Primero llama a __getitem__ sobre la
 * matriz y obtiene una fila. Luego llama a __getitem__ o __setitem__ sobre esa
 * fila. Esta clase cumple ese rol intermedio para cualquier tipo T.
 */
template <typename T>
class MatrixRow {
    private:
        /** @brief Matriz C++ asociada a esta fila. */
        Matrix1<T> *m_matrix = nullptr;
        /** @brief Indice de fila que representa este objeto. */
        size_t      m_row    = 0;

    public:
        /**
         * @brief Construye una vista de fila para una matriz.
         * @param matrix Matriz original.
         * @param row Indice de fila.
         */
        MatrixRow(Matrix1<T> &matrix, size_t row)
            : m_matrix(&matrix), m_row(row) { }

        /**
         * @brief Obtiene el valor de una columna dentro de la fila.
         * @param col Indice de columna.
         * @return Valor almacenado en la celda.
         */
        T Get(size_t col) const {
            return m_matrix->At(m_row, col);
        }

        /**
         * @brief Asigna un valor a una columna dentro de la fila.
         * @param col Indice de columna.
         * @param value Valor a guardar.
         */
        void Set(size_t col, T value) {
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
template <typename T>
static std::string MatrixToString(const Matrix1<T> &matrix) {
    std::ostringstream os;
    Matrix1<T> &mutable_matrix = const_cast<Matrix1<T> &>(matrix);
    mutable_matrix.Print(os);
    return os.str();
}

/**
 * @brief Construye una matriz desde texto usando expresiones regulares.
 *
 * El texto debe separar filas con punto y coma. Ejemplo: "1 2; 3 4".
 *
 * @tparam T Tipo almacenado por la matriz.
 * @param text Texto con los valores de la matriz.
 * @return Matriz construida desde el texto.
 */
template <typename T>
static Matrix1<T> MatrixFromText(const std::string &text) {
    const std::regex row_regex("[^;]+");
    const std::regex number_regex = [] {
        if constexpr (std::is_integral_v<T>)
            return std::regex(R"([-+]?\d+)");
        else
            return std::regex(R"([-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)");
    }();

    std::vector<std::vector<T>> values;

    for (std::sregex_iterator row_it(text.begin(), text.end(), row_regex), end; row_it != end; ++row_it) {
        std::string row_text = row_it->str();
        std::vector<T> row_values;

        for (std::sregex_iterator value_it(row_text.begin(), row_text.end(), number_regex); value_it != end; ++value_it) {
            std::istringstream is(value_it->str());
            T value = T();
            is >> value;
            row_values.push_back(value);
        }

        if (!row_values.empty())
            values.push_back(row_values);
    }

    if (values.empty())
        throw std::invalid_argument("El texto no contiene valores validos para crear una matriz.");

    const size_t cols = values.front().size();
    for (const auto &row : values) {
        if (row.size() != cols)
            throw std::invalid_argument("Todas las filas deben tener la misma cantidad de columnas.");
    }

    Matrix1<T> result(values.size(), cols, T());
    for (size_t i = 0; i < values.size(); ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.At(i, j) = values[i][j];
        }
    }

    return result;
}

/**
 * @brief Multiplica dos matrices usando tareas concurrentes.
 *
 * Cada celda del resultado se calcula en una tarea independiente.
 *
 * @tparam T Tipo almacenado por la matriz.
 * @param lhs Matriz izquierda.
 * @param rhs Matriz derecha.
 * @return Matriz producto.
 */
template <typename T>
static Matrix1<T> MultiplyThreaded(const Matrix1<T> &lhs, const Matrix1<T> &rhs) {
    if (lhs.Cols() != rhs.Rows())
        throw std::invalid_argument("Columnas de la primera matriz deben ser iguales a filas de la segunda matriz.");

    Matrix1<T> result(lhs.Rows(), rhs.Cols(), T());
    std::vector<std::future<void>> tasks;
    tasks.reserve(lhs.Rows() * rhs.Cols());

    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < rhs.Cols(); ++j) {
            tasks.push_back(std::async(std::launch::async, [&lhs, &rhs, &result, i, j] {
                T value = T();
                for (size_t k = 0; k < lhs.Cols(); ++k) {
                    value += lhs.At(i, k) * rhs.At(k, j);
                }
                result.At(i, j) = value;
            }));
        }
    }

    for (auto &task : tasks) {
        task.get();
    }

    return result;
}

/**
 * @brief Registra una especializacion de Matrix1<T> dentro del modulo Python.
 *
 * @tparam T Tipo almacenado por la matriz.
 * @param m Modulo Python donde se registra la clase.
 * @param matrix_name Nombre visible de la matriz en Python.
 * @param row_name Nombre visible de la fila auxiliar en Python.
 */
template <typename T>
void BindMatrix(py::module_ &m, const char *matrix_name, const char *row_name) {
    using MatrixType = Matrix1<T>;
    using RowType = MatrixRow<T>;

    py::class_<RowType>(m, row_name)
        .def("__getitem__", &RowType::Get)
        .def("__setitem__", &RowType::Set);

    py::class_<MatrixType>(m, matrix_name)
        .def(py::init<size_t, size_t, T>(), py::arg("rows"), py::arg("cols"), py::arg("value") = T())
        .def_static("from_text", &MatrixFromText<T>, py::arg("text"),
                    "Construye una matriz desde texto usando expresiones regulares.")
        .def_property_readonly("rows", &MatrixType::Rows)
        .def_property_readonly("cols", &MatrixType::Cols)
        .def("__getitem__", [](MatrixType &matrix, size_t row) {
            return RowType(matrix, row);
        })
        .def("__setitem__", [](MatrixType &matrix, std::pair<size_t, size_t> index, T value) {
            matrix.At(index.first, index.second) = value;
        })
        .def("__str__", &MatrixToString<T>)
        .def("__repr__", &MatrixToString<T>)
        .def("__add__", [](const MatrixType &lhs, const MatrixType &rhs) {
            return lhs + rhs;
        })
        .def("__sub__", [](const MatrixType &lhs, const MatrixType &rhs) {
            return lhs - rhs;
        })
        .def("__mul__", [](const MatrixType &lhs, const MatrixType &rhs) {
            return lhs * rhs;
        })
        .def("__mul__", [](const MatrixType &lhs, T value) {
            return lhs * value;
        })
        .def("__rmul__", [](const MatrixType &rhs, T value) {
            return value * rhs;
        })
        .def("transpose", &MatrixType::Transpose,
             "Devuelve la matriz transpuesta.")
        .def("trace", &MatrixType::Trace,
             "Calcula la traza de una matriz cuadrada.")
        .def("multiply_threaded", [](const MatrixType &lhs, const MatrixType &rhs) {
            return MultiplyThreaded(lhs, rhs);
        }, py::arg("other"), "Multiplica dos matrices usando threads.")
        .def("apply", [](const MatrixType &matrix, py::function func) {
            MatrixType result(matrix.Rows(), matrix.Cols());
            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    result.At(i, j) = func(matrix.At(i, j)).template cast<T>();
                }
            }
            return result;
        }, "Aplica una funcion Python a cada elemento y devuelve una nueva matriz.")
        .def("extra_apply_cpp", [](const MatrixType &matrix, T value) {
            return matrix.Apply([value](const T &current) {
                return current + value;
            });
        }, "Aplica un incremento usando el metodo Apply implementado en C++.");
}

/**
 * @brief Define el modulo Python matrix1_cpp.
 *
 * Este bloque registra MatrixInt, MatrixFloat y Matrix. Matrix se conserva como
 * alias de MatrixInt para mantener compatibilidad con los demos existentes.
 */
PYBIND11_MODULE(matrix1_cpp, m) {
    m.doc() = "Python bindings for the C++ Matrix1<TI> and Matrix1<T3F> classes.";

    BindMatrix<TI>(m, "MatrixInt", "MatrixIntRow");
    BindMatrix<T3F>(m, "MatrixFloat", "MatrixFloatRow");

    m.attr("Matrix") = m.attr("MatrixInt");
}
