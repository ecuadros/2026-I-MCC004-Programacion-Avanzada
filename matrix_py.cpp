#include <pybind11/pybind11.h>
#include <sstream>
#include <stdexcept>
#include "matrix1.h"
#include "types.h"

namespace py = pybind11;

template <typename T>
class MatrixRow
{
    private:
        Matrix1<T> *matrix;
        size_t row;

    public:
        MatrixRow(Matrix1<T> &m, size_t r)
        {
            matrix = &m;
            row = r;
        }

        T get(size_t col) const
        {
            return matrix->Get(row, col);
        }

        void set(size_t col, T value)
        {
            matrix->Set(row, col, value);
        }
};

template <typename T>
void BindMatrix(py::module_ &m, const char *matrix_name, const char *row_name)
{
    using MatrixType = Matrix1<T>;
    using RowType = MatrixRow<T>;

    py::class_<RowType>(m, row_name)
        .def("__getitem__", &RowType::get)
        .def("__setitem__", &RowType::set);

    py::class_<MatrixType>(m, matrix_name)
        .def(py::init<size_t, size_t>())

        .def("set", &MatrixType::Set)
        .def("get", &MatrixType::Get)
        .def("rows", &MatrixType::Rows)
        .def("cols", &MatrixType::Cols)

        .def("__getitem__", [](MatrixType &mat, size_t row) {
            return RowType(mat, row);
        })

        .def("__mul__", [](const MatrixType &a, const MatrixType &b) {
            return a * b;
        })

        .def("__mul__", [](const MatrixType &a, T value) {
            return a * value;
        })

        .def("__rmul__", [](const MatrixType &a, T value) {
            return a * value;
        })

        .def("__add__", [](const MatrixType &a, const MatrixType &b) {
            return a + b;
        })

        .def("__sub__", [](const MatrixType &a, const MatrixType &b) {
            return a - b;
        })

        .def("transpose", [](const MatrixType &mat) {
            MatrixType result(mat.Cols(), mat.Rows());

            for(size_t i = 0; i < mat.Rows(); ++i)
                for(size_t j = 0; j < mat.Cols(); ++j)
                    result.Set(j, i, mat.Get(i, j));

            return result;
        })

        .def("trace", [](const MatrixType &mat) {
            if(mat.Rows() != mat.Cols())
                throw std::invalid_argument("La matriz debe ser cuadrada.");

            T suma = T();

            for(size_t i = 0; i < mat.Rows(); ++i)
                suma = suma + mat.Get(i, i);

            return suma;
        })

        .def("add_value", [](const MatrixType &mat, T value) {
            MatrixType result(mat.Rows(), mat.Cols());

            for(size_t i = 0; i < mat.Rows(); ++i)
                for(size_t j = 0; j < mat.Cols(); ++j)
                    result.Set(i, j, mat.Get(i, j) + value);

            return result;
        })

        .def("__str__", [](MatrixType &mat) {
            std::ostringstream os;
            os << mat;
            return os.str();
        });
}

PYBIND11_MODULE(matrix_py, m)
{
    BindMatrix<TI>(m, "Matrix", "MatrixRow");
    BindMatrix<T3F>(m, "MatrixDecimal", "MatrixDecimalRow");
}