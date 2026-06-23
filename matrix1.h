#ifndef __MATRIX1_H__
#define __MATRIX1_H__
 
#include <algorithm>   // std::swap
#include <cassert>
#include <cstddef>     // size_t
#include <iostream>
#include <stdexcept>   // std::invalid_argument, std::out_of_range
#include <string>
#include <thread>      // std::thread
#include <utility>     // std::exchange
#include <vector>      // std::vector
 
// ---------------------------------------------------------------------
//  Alias de tipos: el curso pide no usar tipos fundamentales "crudos"
//  (bool, int, ...) en el codigo; les damos un nombre con significado.
// ---------------------------------------------------------------------
using BoolType = bool;
 
/**
 * @brief Matriz dinamica de tipo generico T.
 *
 * Guarda los datos en un bloque contiguo (m_data) y mantiene punteros
 * por fila (m_pMat) para conservar la sintaxis m[i][j].
 *
 * @tparam T Tipo de los elementos (por ejemplo double o int).
 */
template <typename T>
class Matrix1 {
private:
    T     *m_data = nullptr;   // bloque contiguo
    T    **m_pMat = nullptr;   // punteros a cada fila (vista 2D)
    size_t m_rows = 0;
    size_t m_cols = 0;
 
    void Create();             // reserva m_data y arma m_pMat
    void Destroy();            // libera memoria
 
public:
    // ---- Constructores / Regla de cinco ----
    Matrix1() = default;
    Matrix1(size_t rows, size_t cols);                       // sin valor (queda en 0)
    /**
     * @brief Crea una matriz rows x cols con todos los elementos en value.
     * @param rows  Numero de filas (debe ser > 0).
     * @param cols  Numero de columnas (debe ser > 0).
     * @param value Valor inicial para cada elemento.
     */
    Matrix1(size_t rows, size_t cols, const T &value);
    Matrix1(const Matrix1 &other);                           // copia profunda
    Matrix1(Matrix1 &&other) noexcept;                       // movimiento
    Matrix1 &operator=(const Matrix1 &other);                // asignacion copia
    Matrix1 &operator=(Matrix1 &&other) noexcept;            // asignacion movimiento
    ~Matrix1() { 
        Destroy(); 
    }
 
    // ---- Fabricas estaticas ----
    static Matrix1 Zeros(size_t rows, size_t cols) { 
        return Matrix1(rows, cols, T{}); 
    }
    static Matrix1 Ones (size_t rows, size_t cols) { 
        return Matrix1(rows, cols, T{1}); 
    }
 
    // ---- Acceso ----
    size_t   Rows() const { 
        return m_rows; 
    }
    size_t   Cols() const { 
        return m_cols; 
    }
    T       *Data()       { 
        return m_data; 
    }   // bloque contiguo (para NumPy)
    const T *Data() const { 
        return m_data; 
    }
 
    T       *operator[](size_t row);            // devuelve la fila -> permite m[i][j]
    const T *operator[](size_t row) const;
    T       &At(size_t i, size_t j);            // acceso validado por AMBOS indices
    const T &At(size_t i, size_t j) const;
 
    // ---- IO ----
    std::istream &Read(std::istream &is);
    std::ostream &Print(std::ostream &os) const;
 
    // ---- Comparacion ----
    BoolType operator==(const Matrix1 &other) const;
    BoolType operator!=(const Matrix1 &other) const { 
        return !(*this == other); 
    }
 
    // ---- Matriz-matriz ----
    Matrix1  operator+ (const Matrix1 &other) const;
    Matrix1  operator- (const Matrix1 &other) const;
    /**
     * @brief Producto matricial (filas por columnas).
     * @param other Matriz de la derecha; sus filas deben igualar las columnas de esta.
     * @return Matriz de dimension (filas de esta x columnas de other).
     */
    Matrix1  operator* (const Matrix1 &other) const;
    Matrix1 &operator+=(const Matrix1 &other);
    Matrix1 &operator-=(const Matrix1 &other);
    Matrix1 &operator*=(const Matrix1 &other);
 
    /**
     * @brief Igual que operator*, pero repartiendo las filas entre varios hilos.
     * @param other    Matriz de la derecha.
     * @param nThreads Numero de hilos; 0 usa los nucleos disponibles.
     * @return Matriz resultado del producto.
     */
    Matrix1 MultiplyConcurrent(const Matrix1 &other, unsigned nThreads = 0) const;
 
    // ---- Matriz-escalar ----
    Matrix1  operator+ (const T &s) const;
    Matrix1  operator- (const T &s) const;
    Matrix1  operator* (const T &s) const;
    Matrix1 &operator+=(const T &s);
    Matrix1 &operator-=(const T &s);
    Matrix1 &operator*=(const T &s);
 
    // ---- Elemento a elemento (Hadamard) ----
    Matrix1 ElementWiseMultiply(const Matrix1 &other) const;
 
    // ---- Aplicar una funcion a los elementos (factorizan los bucles) ----
    // func(elemento)           : transforma cada elemento in situ
    template <typename Func>
    Matrix1 &ApplyInPlace(Func func);
    // func(elemento, elemento_de_other) : combina con otra matriz, in situ
    template <typename Func>
    Matrix1 &ApplyElementWise(const Matrix1 &other, Func func);
 
    // ---- Extras (implementados en C++, usados desde Python) ----
    /** @brief Matriz identidad n x n. */
    static Matrix1 Identity(size_t n);
    /** @brief Devuelve la transpuesta (el elemento i,j pasa a j,i). */
    Matrix1        Transpose() const;
    /** @brief Suma de la diagonal principal; requiere matriz cuadrada. */
    T              Trace() const;
};
 
// ---------------------------------------------------------------------
//  Memoria
// ---------------------------------------------------------------------
template <typename T>
void Matrix1<T>::Create() {
    assert(m_rows > 0 && m_cols > 0);
    m_data = new T[m_rows * m_cols]();          // () => valor-inicializa a 0
    m_pMat = new T*[m_rows];
    for (size_t i = 0; i < m_rows; ++i)
        m_pMat[i] = m_data + i * m_cols;        // cada fila apunta dentro del bloque
}
 
template <typename T>
void Matrix1<T>::Destroy() {
    delete[] m_data;
    delete[] m_pMat;
    m_data = nullptr;
    m_pMat = nullptr;
    m_rows = 0;
    m_cols = 0;
}
 
// ---------------------------------------------------------------------
//  Constructores
// ---------------------------------------------------------------------
template <typename T>
Matrix1<T>::Matrix1(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
    if (rows == 0 || cols == 0)
        throw std::invalid_argument("Dimensiones invalidas para Matrix1");
    Create();
}
 
template <typename T>
Matrix1<T>::Matrix1(size_t rows, size_t cols, const T &value) : m_rows(rows), m_cols(cols) {
    if (rows == 0 || cols == 0)
        throw std::invalid_argument("Dimensiones invalidas para Matrix1");
    Create();
    for (size_t k = 0; k < m_rows * m_cols; ++k)
        m_data[k] = value;
}
 
template <typename T>
Matrix1<T>::Matrix1(const Matrix1 &other) : m_rows(other.m_rows), m_cols(other.m_cols) {
    if (other.m_data == nullptr) {
         m_rows = m_cols = 0; return; 
        }
    Create();
    for (size_t k = 0; k < m_rows * m_cols; ++k)
        m_data[k] = other.m_data[k];
}
 
template <typename T>
Matrix1<T>::Matrix1(Matrix1 &&other) noexcept
    : m_data(std::exchange(other.m_data, nullptr)),
      m_pMat(std::exchange(other.m_pMat, nullptr)),
      m_rows(std::exchange(other.m_rows, 0)),
      m_cols(std::exchange(other.m_cols, 0)) {}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator=(const Matrix1 &other) {
    if (this != &other) {                       // copy-and-swap
        Matrix1 tmp(other);
        std::swap(m_data, tmp.m_data);
        std::swap(m_pMat, tmp.m_pMat);
        std::swap(m_rows, tmp.m_rows);
        std::swap(m_cols, tmp.m_cols);
    }
    return *this;
}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator=(Matrix1 &&other) noexcept {
    if (this != &other) {
        Destroy();
        m_data = std::exchange(other.m_data, nullptr);
        m_pMat = std::exchange(other.m_pMat, nullptr);
        m_rows = std::exchange(other.m_rows, 0);
        m_cols = std::exchange(other.m_cols, 0);
    }
    return *this;
}
 
// ---------------------------------------------------------------------
//  Acceso
// ---------------------------------------------------------------------
template <typename T>
T *Matrix1<T>::operator[](size_t row) {
    if (row >= m_rows) throw std::out_of_range("Indice de fila fuera de rango");
    return m_pMat[row];
}
 
template <typename T>
const T *Matrix1<T>::operator[](size_t row) const {
    if (row >= m_rows) throw std::out_of_range("Indice de fila fuera de rango");
    return m_pMat[row];
}
 
template <typename T>
T &Matrix1<T>::At(size_t i, size_t j) {
    if (i >= m_rows || j >= m_cols) throw std::out_of_range("Indice fuera de rango");
    return m_pMat[i][j];
}
 
template <typename T>
const T &Matrix1<T>::At(size_t i, size_t j) const {
    if (i >= m_rows || j >= m_cols) throw std::out_of_range("Indice fuera de rango");
    return m_pMat[i][j];
}
 
// ---------------------------------------------------------------------
//  IO   (formato:  MATRIZ:  R C  seguido de los R*C valores)
// ---------------------------------------------------------------------
template <typename T>
std::istream &Matrix1<T>::Read(std::istream &is) {
    Destroy();
    std::string token;
    is >> token;
    if (!is) return is;
    if (token != "MATRIZ:" && token != "MATRIZ") {
        is.setstate(std::ios::failbit);
        return is;
    }
    size_t r = 0, c = 0;
    is >> r >> c;
    if (!is || r == 0 || c == 0) {
        is.setstate(std::ios::failbit);
        return is;
    }
    m_rows = r; m_cols = c;
    Create();
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            if (!(is >> m_pMat[i][j])) { Destroy(); return is; }
    return is;
}
 
template <typename T>
std::ostream &Matrix1<T>::Print(std::ostream &os) const {
    os << "MATRIZ:\n" << m_rows << " " << m_cols << "\n";
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j)
            os << m_pMat[i][j] << " ";
        os << "\n";
    }
    return os;
}
 
// ---------------------------------------------------------------------
//  Comparacion
// ---------------------------------------------------------------------
template <typename T>
BoolType Matrix1<T>::operator==(const Matrix1 &other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) return false;
    for (size_t k = 0; k < m_rows * m_cols; ++k)
        if (m_data[k] != other.m_data[k]) return false;
    return true;
}
 
// ---------------------------------------------------------------------
//  Aplicar funcion a los elementos
// ---------------------------------------------------------------------
template <typename T>
template <typename Func>
Matrix1<T> &Matrix1<T>::ApplyInPlace(Func func) {
    for (size_t k = 0; k < m_rows * m_cols; ++k)
        func(m_data[k]);
    return *this;
}
 
template <typename T>
template <typename Func>
Matrix1<T> &Matrix1<T>::ApplyElementWise(const Matrix1 &other, Func func) {
    if (m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Operacion elemento a elemento: dimensiones distintas");
    for (size_t k = 0; k < m_rows * m_cols; ++k)
        func(m_data[k], other.m_data[k]);
    return *this;
}
 
// ---------------------------------------------------------------------
//  Matriz-matriz
// ---------------------------------------------------------------------
template <typename T>
Matrix1<T> &Matrix1<T>::operator+=(const Matrix1 &other) {
    return ApplyElementWise(other, [](T &x, const T &y) { x += y; });
}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator-=(const Matrix1 &other) {
    return ApplyElementWise(other, [](T &x, const T &y) { x -= y; });
}
 
template <typename T>
Matrix1<T> Matrix1<T>::operator+(const Matrix1 &other) const {
    Matrix1 r(*this); r += other; return r;
}
 
template <typename T>
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const {
    Matrix1 r(*this); r -= other; return r;
}
 
template <typename T>
Matrix1<T> Matrix1<T>::operator*(const Matrix1 &other) const {
    if (m_cols != other.m_rows)
        throw std::invalid_argument("Multiplicacion: columnas(A) != filas(B)");
    Matrix1 r(m_rows, other.m_cols, T{});
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < other.m_cols; ++j) {
            T acc = T{};
            for (size_t k = 0; k < m_cols; ++k)
                acc += m_pMat[i][k] * other.m_pMat[k][j];
            r.m_pMat[i][j] = acc;
        }
    return r;
}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator*=(const Matrix1 &other) {
    *this = (*this) * other;
    return *this;
}
 
// Multiplicacion concurrente: cada hilo calcula un rango de filas del
// resultado. Como las filas no se solapan, NO hace falta mutex.
template <typename T>
Matrix1<T> Matrix1<T>::MultiplyConcurrent(const Matrix1 &other, unsigned nThreads) const {
    if (m_cols != other.m_rows)
        throw std::invalid_argument("Multiplicacion: columnas(A) != filas(B)");
 
    Matrix1 result(m_rows, other.m_cols, T{});
 
    if (nThreads == 0) nThreads = std::thread::hardware_concurrency();
    if (nThreads == 0) nThreads = 1;                 // por si hardware_concurrency devuelve 0
    if (nThreads > m_rows) nThreads = m_rows;        // no mas hilos que filas
 
    // Cada hilo llena las filas [inicio, fin) del resultado.
    auto worker = [&](size_t inicio, size_t fin) {
        for (size_t i = inicio; i < fin; ++i)
            for (size_t j = 0; j < other.m_cols; ++j) {
                T acc = T{};
                for (size_t k = 0; k < m_cols; ++k)
                    acc += m_pMat[i][k] * other.m_pMat[k][j];
                result.m_pMat[i][j] = acc;           // fila i: exclusiva de este hilo
            }
    };
 
    std::vector<std::thread> hilos;
    const size_t base  = m_rows / nThreads;          // filas por hilo
    const size_t resto = m_rows % nThreads;          // filas extra a repartir
 
    size_t inicio = 0;
    for (unsigned t = 0; t < nThreads; ++t) {
        size_t fin = inicio + base + (t < resto ? 1 : 0);
        hilos.emplace_back(worker, inicio, fin);
        inicio = fin;
    }
    for (auto &h : hilos) h.join();                  // esperar a todos
 
    return result;
}
 
// ---------------------------------------------------------------------
//  Matriz-escalar
// ---------------------------------------------------------------------
template <typename T>
Matrix1<T> &Matrix1<T>::operator+=(const T &s) {
    return ApplyInPlace([&s](T &x) { x += s; });
}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator-=(const T &s) {
    return ApplyInPlace([&s](T &x) { x -= s; });
}
 
template <typename T>
Matrix1<T> &Matrix1<T>::operator*=(const T &s) {
    return ApplyInPlace([&s](T &x) { x *= s; });
}
 
template <typename T>
Matrix1<T> Matrix1<T>::operator+(const T &s) const { Matrix1 r(*this); r += s; return r; }
 
template <typename T>
Matrix1<T> Matrix1<T>::operator-(const T &s) const { Matrix1 r(*this); r -= s; return r; }
 
template <typename T>
Matrix1<T> Matrix1<T>::operator*(const T &s) const { Matrix1 r(*this); r *= s; return r; }
 
// ---------------------------------------------------------------------
//  Elemento a elemento
// ---------------------------------------------------------------------
template <typename T>
Matrix1<T> Matrix1<T>::ElementWiseMultiply(const Matrix1 &other) const {
    Matrix1 r(*this);
    r.ApplyElementWise(other, [](T &x, const T &y) { x *= y; });
    return r;
}
 
// ---------------------------------------------------------------------
//  Extras
// ---------------------------------------------------------------------
// Extra 1: matriz identidad n x n (1 en la diagonal, 0 en el resto).
template <typename T>
Matrix1<T> Matrix1<T>::Identity(size_t n) {
    Matrix1 r(n, n, T{});
    for (size_t i = 0; i < n; ++i)
        r.m_pMat[i][i] = T{1};
    return r;
}
 
// Extra 2: transpuesta. El elemento (i, j) pasa a la posicion (j, i).
template <typename T>
Matrix1<T> Matrix1<T>::Transpose() const {
    Matrix1 r(m_cols, m_rows, T{});
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            r.m_pMat[j][i] = m_pMat[i][j];
    return r;
}
 
// Extra 3: traza = suma de la diagonal principal (solo matrices cuadradas).
template <typename T>
T Matrix1<T>::Trace() const {
    if (m_rows != m_cols)
        throw std::invalid_argument("La traza solo esta definida para matrices cuadradas");
    T acc = T{};
    for (size_t i = 0; i < m_rows; ++i)
        acc += m_pMat[i][i];
    return acc;
}
 
// ---------------------------------------------------------------------
//  Operadores libres
// ---------------------------------------------------------------------
template <typename T>
std::istream &operator>>(std::istream &is, Matrix1<T> &m) { return m.Read(is); }
 
template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix1<T> &m) { return m.Print(os); }
 
template <typename T>
Matrix1<T> operator+(const T &s, const Matrix1<T> &m) { return m + s; }
 
template <typename T>
Matrix1<T> operator*(const T &s, const Matrix1<T> &m) { return m * s; }
 
template <typename T>
Matrix1<T> operator-(const T &s, const Matrix1<T> &m) {
    Matrix1<T> r(m.Rows(), m.Cols(), T{});
    for (size_t i = 0; i < m.Rows(); ++i)
        for (size_t j = 0; j < m.Cols(); ++j)
            r[i][j] = s - m[i][j];
    return r;
}
 
#endif // __MATRIX1_H__