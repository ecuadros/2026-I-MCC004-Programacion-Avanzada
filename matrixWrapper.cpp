#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <string>
#include "matrix1.h"

namespace py = pybind11;

// Estructura Proxy para soportar la sintaxis m[i][j] en Python
template <typename T>
struct RowProxy {
    T* row_ptr;
    T getitem(size_t c) const { return row_ptr[c]; }
    void setitem(size_t c, T val) { row_ptr[c] = val; }
};

// Función auxiliar para redirigir tu método Print() a un string para Python
template <typename T>
std::string MatrixToString(Matrix1<T>& m) {
    std::stringstream ss;
    m.Print(ss);
    return ss.str();
}

// Implementando de manera generica...
template <typename T>
void declare_matrix(py::module &m, const std::string &typestr) {
	std::string pyclass_name = "Matrix" + typestr; // Para usar MatrixInt, MatrixFloat
	std::string proxy_name = "_RowProxy" + typestr; 
	
	py::class_<RowProxy<T>>(m, proxy_name.c_str())
		.def("__getitem__", &RowProxy<T>::getitem)
		.def("__setitem__", &RowProxy<T>::setitem);
		
	py::class_<Matrix1<T>>(m, pyclass_name.c_str())
		.def(py::init<size_t, size_t>())
		.def("create", &Matrix1<T>::Create)
		.def("destroy", &Matrix1<T>::Destroy)
		.def("diag", &Matrix1<T>::Diag)
		.def("transpose", &Matrix1<T>::Transpose)
		.def("det", &Matrix1<T>::Det)
		
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * py::self)
		.def(py::self * T())
		
		.def("__str__", &MatrixToString<T>)
		
		.def("__getitem__", [](Matrix1<T> &instance, size_t r){
			return RowProxy<T> {instance[r]};
		})
		
		.def("__getitem__", [](Matrix1<T> &instance, std::pair<size_t, size_t> idx){
			return instance(idx.first, idx.second);
		})
		
		.def("__setitem__", [](Matrix1<T> &instance, std::pair<size_t, size_t> idx, T val) {
            instance(idx.first, idx.second) = val;
        });
}



//----- Modulo principal -> pybind11
PYBIND11_MODULE(matrix_lib, m) { // matrix_lib : Nombre de la libreria, m : libreria de c++
    m.doc() = "Librería de matrices en C++ expuesta a Python";

    declare_matrix<int>(m, "Int");
    declare_matrix<float>(m, "Float");
    declare_matrix<double>(m, "Double");
        
}
