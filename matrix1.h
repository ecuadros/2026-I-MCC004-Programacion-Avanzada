/**
 * @file matrix1.h
 * @brief Definición de la clase Matrix1 para matrices dinámicas.
 * @author Tu Nombre
 * @date 2026
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <functional>
#include <iostream>
#include <sstream>
#include <cassert>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <string>

using namespace std;

/**
 * @brief Clase plantilla para matrices de tamaño dinámico.
 * 
 * @tparam T Tipo de los elementos de la matriz. Debe soportar operaciones aritméticas básicas.
 * 
 * Esta clase proporciona gestión de memoria, operadores aritméticos, 
 * acceso a elementos y cálculo del determinante.
 */

template <typename T>
class Matrix1 {
    private:
        T      **m_pMat = nullptr;
        size_t   m_rows = 0;
        size_t   m_cols = 0;

    public:
    /** @brief Constructor por defecto: matriz vacía (0x0). */
        Matrix1()      { };
                /**
         * @brief Constructor de copia.
         * @param other Matriz a copiar.
         */
        Matrix1(const Matrix1 &other) ;
                /**
         * @brief Constructor de movimiento.
         * @param other Matriz a mover (roba sus recursos).
         */
        Matrix1(Matrix1 &&other); 
         /** @brief Destructor. Libera la memoria. */
        ~Matrix1()     { Destroy(); }
         /**
         * @brief Crea la matriz en el heap con las dimensiones actuales.
         * @pre m_rows > 0 y m_cols > 0.
         * @post m_pMat apunta a memoria válida.
         */
        void     Create();
                /**
         * @brief Lee la matriz desde un flujo de entrada.
         * @param is Flujo de entrada (por ejemplo, std::ifstream).
         * @return Referencia al mismo flujo.
         * @note El formato esperado: "filas columnas\\n" seguido de los datos.
         */
        istream &Read(istream &is);
                /**
         * @brief Aplica una función a cada elemento de la matriz.
         * @tparam Func Tipo de la función (debe aceptar T&, size_t, size_t y args...).
         * @tparam Args Tipos de los argumentos adicionales.
         * @param func Función a aplicar.
         * @param args Argumentos adicionales (forwardeados).
         * 
         * Ejemplo: ApplyFunctionToAllMatriz([](T& elem, size_t r, size_t c, int x){ elem += x; }, 10);
         */
        template <typename Func, typename... Args>
        void ApplyFunctionToAllMatriz(Func func, Args&&... args);
                /**
         * @brief Imprime la matriz en un flujo de salida.
         * @param os Flujo de salida (std::cout, std::ofstream, etc.).
         * @return Referencia al mismo flujo.
         */
        ostream &Print(ostream &os);
        /** @return Número de filas. */
        size_t   getRows() const { return m_rows; }
        /** @return Número de columnas. */
        size_t   getCols() const { return m_cols; }
        /**
         * @brief Construye una matriz con dimensiones dadas.
         * @param rows Número de filas.
         * @param cols Número de columnas.
         * @throw std::invalid_argument si rows==0 o cols==0 (aunque el assert lo captura).
         */

        Matrix1(size_t rows, size_t cols); 
        /**
         * @brief Suma de matrices elemento a elemento.
         * @param other Matriz a sumar.
         * @return Nueva matriz con la suma.
         * @throw std::invalid_argument si las dimensiones no coinciden.
         */
        Matrix1 operator+(const Matrix1 &other) const;
        /**
         * @brief Resta de matrices elemento a elemento.
         * @param other Matriz a restar.
         * @return Nueva matriz con la resta.
         * @throw std::invalid_argument si las dimensiones no coinciden.
         */
        Matrix1 operator-(const Matrix1 &other) const;
        /**
         * @brief Multiplicación por un escalar (cada elemento).
         * @param value Escalar a multiplicar.
         * @return Nueva matriz con todos los elementos multiplicados por value.
         */
        Matrix1 operator*(T value) const;
        /**
         * @brief Multiplicación matricial (producto de matrices).
         * @param other Matriz a multiplicar.
         * @return Nueva matriz resultado del producto.
         * @throw std::invalid_argument si m_cols != other.m_rows.
         */
        Matrix1 operator*(const Matrix1& other) const;
        /**
         * @brief División por un escalar (cada elemento).
         * @param value Escalar divisor.
         * @return Nueva matriz con todos los elementos divididos por value.
         * @throw std::invalid_argument si value == 0.
         */
        Matrix1 operator/(T value) const;
        /**
         * @brief Calcula el determinante de la matriz.
         * @return El valor del determinante.
         * @throw std::invalid_argument si la matriz no es cuadrada.
         * @note Utiliza eliminación gaussiana con pivoteo parcial.
         */
        T operatorDet() const;
        /**
         * @brief Acceso (lectura/escritura) a un elemento.
         * @param row Índice de fila (base 0).
         * @param col Índice de columna (base 0).
         * @return Referencia al elemento.
         * @throw std::out_of_range si índices inválidos (via assert en debug).
         */
        T& operator()(size_t row, size_t col);
        /**
         * @brief Acceso de solo lectura a un elemento.
         * @param row Índice de fila.
         * @param col Índice de columna.
         * @return Referencia constante al elemento.
         * @throw std::out_of_range si índices inválidos.
         */
        const T& operator()(size_t row, size_t col) const;
        /** @brief Libera la memoria y reinicia las dimensiones a 0. */


        T* operator[](size_t row);
        const T* operator[](size_t row) const;


        void Destroy();
};

template <typename T>
void PrintMatriz(T &elem, size_t row, size_t col, ostream &os) {
    os << "[" << row << "][" << col << "] = " << elem << "  ";
}

template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

template <typename T>
Matrix1<T>::Matrix1(size_t rows, size_t cols) 
    : m_rows(rows), m_cols(cols) {
    Create();
}

template <typename T>
istream &Matrix1<T>::Read(istream &is) {
    Destroy();
    is >> m_rows >> m_cols;
    Create();
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            is >> m_pMat[i][j];
    return is;
}
// 
template <typename T>
template <typename Func, typename... Args>
void Matrix1<T>::ApplyFunctionToAllMatriz(Func func, Args&&... args) {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            func(m_pMat[i][j], i, j, forward<Args>(args)...);
        }
    }
}


template <typename T>
ostream &Matrix1<T>::Print(ostream &os) {
    os << m_rows << " " << m_cols << "\n";
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j)
            os << m_pMat[i][j] << " ";
        os << "\n";
    }
    return os;
}

template <typename T>
void Matrix1<T>::Destroy() {
    if (m_pMat != nullptr) {
        for (size_t i = 0; i < m_rows; ++i)
            delete[] m_pMat[i];
        delete[] m_pMat;
        m_pMat = nullptr;
    }
    m_rows = m_cols = 0;
}

// Operadores sobrecargados
template <typename T>
istream &operator>>(istream &is, Matrix1<T> &mat) {
    return mat.Read(is);
}

template <typename T>
ostream &operator<<(ostream &os, Matrix1<T> &mat) {
   return mat.Print(os);
}

// Move constructor
template <typename T> 
Matrix1<T>::Matrix1(Matrix1 &&other) {
    m_pMat = exchange(other.m_pMat, nullptr);
    m_rows = exchange(other.m_rows, 0);
    m_cols = exchange(other.m_cols, 0);
}

//Copy Constructor
template <typename T>
Matrix1<T>::Matrix1(const Matrix1 &other){
    m_rows = other.m_rows; 
    m_cols = other.m_cols; 
    Create(); 
    for (size_t i = 0; i < m_rows; ++i) 
        for (size_t j = 0; j < m_cols; ++j) 
            m_pMat[i][j] = other.m_pMat[i][j]; 
}

template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1 &other) const
{
    
    if (m_rows != other.m_rows || m_cols != other.m_cols) throw invalid_argument("Dimensiones incompatibles");

    Matrix1<T> suma(m_rows, m_cols); 
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            suma.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
        }
    }
    return suma;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const
{

    if (m_rows != other.m_rows || m_cols != other.m_cols) throw invalid_argument("Dimensiones incompatibles para resta.");
    
    Matrix1<T> resta(m_rows, m_cols); 
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            resta.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
        }
    }

    return resta;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator*(T value) const
{
    Matrix1<T> result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result.m_pMat[i][j] = m_pMat[i][j] * value;
    return result;
}

template <typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1& other) const {
    if (m_cols != other.m_rows) throw invalid_argument("Dimensiones incompatibles para multiplicación.");
        
    Matrix1<T> result(m_rows, other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            T sum = 0;
            for (size_t k = 0; k < m_cols; ++k)
                sum += m_pMat[i][k] * other.m_pMat[k][j];
            result.m_pMat[i][j] = sum;
        }
    }
    return result;
}


template <typename T>
T& Matrix1<T>::operator()(size_t row, size_t col) {
    assert(row < m_rows && col < m_cols);
    return m_pMat[row][col];
}

template <typename T>
const T& Matrix1<T>::operator()(size_t row, size_t col) const {
    assert(row < m_rows && col < m_cols);
    return m_pMat[row][col];
}

template <typename T>
string MatrizToString(const Matrix1<T>& mat) {
    ostringstream oss;
    oss << mat.getRows() << " " << mat.getCols() << "\n";
    for (size_t i = 0; i < mat.getRows(); ++i) {
        for (size_t j = 0; j < mat.getCols(); ++j) {
            oss << mat(i, j) << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

/// @brief 
/// @tparam T 
/// @param value 
/// @return 
template <typename T>
Matrix1<T> Matrix1<T>::operator/(T value) const {
    if (value == 0) throw invalid_argument("División por cero");

    Matrix1<T> result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            result.m_pMat[i][j] = m_pMat[i][j] / value;
        }
    }
    return result;
}

template <typename T>
T Matrix1<T>::operatorDet() const {
    if (m_rows != m_cols)
        throw invalid_argument("La matriz no es cuadrada");

    // Copia de la matriz usando constructor de copia (const)
    Matrix1<T> temp(*this);
    T det = 1;
    const size_t n = m_rows;

    for (size_t i = 0; i < n; ++i) {
        // Pivoteo parcial
        size_t pivot = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (abs(temp.m_pMat[j][i]) > abs(temp.m_pMat[pivot][i]))
                pivot = j;
        }

        if (temp.m_pMat[pivot][i] == 0)
            return T(0);

        if (pivot != i) {
            std::swap(temp.m_pMat[i], temp.m_pMat[pivot]);
            det = -det;
        }

        // Eliminación gaussiana
        for (size_t j = i + 1; j < n; ++j) {
            T factor = temp.m_pMat[j][i] / temp.m_pMat[i][i];
            for (size_t k = i; k < n; ++k)
                temp.m_pMat[j][k] -= factor * temp.m_pMat[i][k];
        }
    }

    // Producto de la diagonal
    for (size_t i = 0; i < n; ++i)
        det *= temp.m_pMat[i][i];

    return det;
}

template <typename T>
T* Matrix1<T>::operator[](size_t row) {
    assert(row < m_rows);
    return m_pMat[row];
}

template <typename T>
const T* Matrix1<T>::operator[](size_t row) const {
    assert(row < m_rows);
    return m_pMat[row];
}

#endif // __MATRIX_H__