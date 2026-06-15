#ifndef __MATRIX_H__
#define __MATRIX_H__
// #include <__nullptr>
#include <functional>
#include <iostream>
#include <cassert>

using namespace std;

// template <typename T>
// void Print4(T &n, ostream &os) { os << n << " "; }

template <typename T>
/**
 * @class Matrix1
 * @brief Matriz dinamica generica implementada con doble puntero.
 *
 * Esta clase administra una matriz en memoria dinamica usando un arreglo de
 * punteros a filas. Permite crear, leer, imprimir, copiar, mover y operar
 * matrices con suma, resta, multiplicacion matricial y multiplicacion escalar.
 *
 * @tparam T Tipo de dato almacenado en cada celda de la matriz.
 */
class Matrix1 {
    private:
        /** @brief Puntero principal a las filas de la matriz. */
        T      **m_pMat = nullptr;
        /** @brief Cantidad de filas y columnas de la matriz. */
        size_t   m_rows = 0, m_cols = 0;
        /**
         * @brief Recorre todos los indices validos de la matriz.
         *
         * La funcion recibida debe aceptar dos parametros: fila y columna.
         *
         * @tparam Func Tipo de la funcion o lambda a ejecutar.
         * @param func Funcion que se ejecuta para cada posicion (i, j).
         */
        template <typename Func>
        void ForEachIndex(Func func) const;
    public:
        /** @brief Constructor por defecto. Crea una matriz vacia. */
        Matrix1()      { }
        /**
         * @brief Constructor por dimensiones.
         *
         * Reserva memoria para una matriz de rows x cols e inicializa todas las
         * celdas con value.
         *
         * @param rows Numero de filas.
         * @param cols Numero de columnas.
         * @param value Valor inicial de cada celda.
         */
        Matrix1(size_t rows, size_t cols, const T &value = T());
        /** @brief Destructor. Libera la memoria dinamica de la matriz. */
        ~Matrix1()     { Destroy(); }
        /**
         * @brief Constructor de movimiento.
         *
         * Transfiere la memoria dinamica desde other hacia el objeto actual y
         * deja other en estado vacio.
         *
         * @param other Matriz temporal u objeto a mover.
         */
        Matrix1(Matrix1 &&other) noexcept; // Move constructor
        /**
         * @brief Constructor de copia.
         *
         * Realiza una copia profunda de la matriz recibida.
         *
         * @param other Matriz origen.
         */
        Matrix1(const Matrix1 &other); // Copy Constructor
 
        /**
         * @brief Operador de asignacion por copia.
         * @param other Matriz origen.
         * @return Referencia al objeto actual.
         */
        Matrix1<T> &operator=(const Matrix1<T> &other); // Operator = - Copy assignment operator
        /**
         * @brief Operador de asignacion por movimiento.
         * @param other Matriz origen a mover.
         * @return Referencia al objeto actual.
         */
        Matrix1<T> &operator=(Matrix1<T> &&other) noexcept; // Operator = - Move assignment operator

        /**
         * @brief Suma dos matrices de las mismas dimensiones.
         * @param other Matriz a sumar.
         * @return Matriz resultado.
         */
        Matrix1<T> operator+(const Matrix1<T> &other) const; // Operator +
        /**
         * @brief Resta dos matrices de las mismas dimensiones.
         * @param other Matriz a restar.
         * @return Matriz resultado.
         */
        Matrix1<T> operator-(const Matrix1<T> &other) const; // Operator -
        /**
         * @brief Multiplica dos matrices compatibles.
         * @param other Matriz del lado derecho.
         * @return Matriz resultado.
         */
        Matrix1<T> operator*(const Matrix1<T> &other) const; // Operator *
        /**
         * @brief Multiplica todos los elementos por un valor escalar.
         * @param value Valor escalar.
         * @return Matriz resultado.
         */
        Matrix1<T> operator*(T value) const; // Operator * escalar
        /**
         * @brief Devuelve la matriz transpuesta.
         *
         * Las filas de la matriz original pasan a ser columnas en la matriz
         * resultado.
         *
         * @return Matriz transpuesta.
         */
        Matrix1<T> Transpose() const;
        /**
         * @brief Aplica una funcion a cada celda y devuelve una nueva matriz.
         *
         * No modifica la matriz original. La funcion recibe el valor actual de
         * cada celda y retorna el nuevo valor.
         *
         * @param func Funcion a aplicar.
         * @return Matriz resultado.
         */
        Matrix1<T> Apply(function<T(const T&)> func) const;
        /**
         * @brief Calcula la traza de una matriz cuadrada.
         *
         * La traza es la suma de los elementos de la diagonal principal.
         *
         * @return Valor de la traza.
         */
        T Trace() const;

        /** @brief Reserva memoria dinamica para la matriz. */
        void Create();
        /** @brief Devuelve la cantidad de filas. */
        size_t Rows() const;
        /** @brief Devuelve la cantidad de columnas. */
        size_t Cols() const;
        /**
         * @brief Accede a una celda modificable.
         * @param row Fila.
         * @param col Columna.
         * @return Referencia a la celda solicitada.
         */
        T &At(size_t row, size_t col);
        /**
         * @brief Accede a una celda de solo lectura.
         * @param row Fila.
         * @param col Columna.
         * @return Referencia constante a la celda solicitada.
         */
        const T &At(size_t row, size_t col) const;
        /**
         * @brief Lee dimensiones y valores desde un stream.
         * @param is Stream de entrada.
         * @return Referencia al stream recibido.
         */
        istream &Read(istream &is);
        /**
         * @brief Aplica una funcion a cada elemento de la matriz.
         *
         * Permite enviar argumentos extra a la funcion recibida.
         *
         * @tparam Func Tipo de funcion.
         * @tparam Args Tipos de argumentos extra.
         * @param func Funcion a aplicar.
         * @param args Argumentos extra.
         */
        template <typename Func, typename... Args>
        void ApplyFunctionToAll(Func func, Args&& ...args);
        /**
         * @brief Imprime la matriz en un stream.
         * @param os Stream de salida.
         * @return Referencia al stream recibido.
         */
        ostream &Print(ostream &os);
        /** @brief Libera toda la memoria dinamica de la matriz. */
        void Destroy();
};

/**
 * @brief Reserva memoria para una matriz con las dimensiones ya configuradas.
 *
 * Crea un arreglo de punteros para las filas y luego reserva un arreglo para
 * cada fila.
 */
template <typename T>
void Matrix1<T>::Create()
{   assert(m_rows > 0 && m_cols > 0);
    m_pMat = new T *[m_rows];
    for(size_t i = 0 ; i < m_rows ; ++i)
        m_pMat[i] = new T[m_cols];
}

template <typename T>
/**
 * @brief Construye una matriz con dimensiones y valor inicial.
 */
Matrix1<T>::Matrix1(size_t rows, size_t cols, const T &value)
    : m_rows(rows), m_cols(cols) {
    Create();
    ForEachIndex([&](size_t i, size_t j) {
        m_pMat[i][j] = value;
    });
}

template <typename T>
template <typename Func>
/**
 * @brief Ejecuta una funcion para cada par de indices (fila, columna).
 */
void Matrix1<T>::ForEachIndex(Func func) const {
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            func(i, j);
}

// Copy Constructor
template <typename T>
/**
 * @brief Crea una nueva matriz copiando profundamente otra matriz.
 */
Matrix1<T>::Matrix1(const Matrix1 &other) {
    m_rows = other.m_rows;
    m_cols = other.m_cols;

    if(other.m_pMat != nullptr) {
    Create();

    ForEachIndex([&](size_t i, size_t j) {
        m_pMat[i][j] = other.m_pMat[i][j];
    });
    }
}

// Move constructor
template <typename T>
/**
 * @brief Mueve los recursos de otra matriz usando std::exchange.
 */
Matrix1<T>::Matrix1(Matrix1 &&other) noexcept {
    m_pMat = std::exchange(other.m_pMat, nullptr);
    m_rows = std::exchange(other.m_rows, 0);
    m_cols = std::exchange(other.m_cols, 0);
}

template <typename T>
/**
 * @brief Devuelve el numero de filas.
 */
size_t Matrix1<T>::Rows() const {
    return m_rows;
}

template <typename T>
/**
 * @brief Devuelve el numero de columnas.
 */
size_t Matrix1<T>::Cols() const {
    return m_cols;
}

template <typename T>
/**
 * @brief Devuelve una referencia modificable a una celda.
 */
T &Matrix1<T>::At(size_t row, size_t col) {
    if (row >= m_rows || col >= m_cols)
        throw std::out_of_range("Matrix index out of range.");
    return m_pMat[row][col];
}

template <typename T>
/**
 * @brief Devuelve una referencia constante a una celda.
 */
const T &Matrix1<T>::At(size_t row, size_t col) const {
    if (row >= m_rows || col >= m_cols)
        throw std::out_of_range("Matrix index out of range.");
    return m_pMat[row][col];
}

template <typename T>
/**
 * @brief Lee una matriz desde un stream.
 */
istream &Matrix1<T>::Read(istream &is) {
    Destroy();
    is >> m_rows >> m_cols;
    Create();
    ForEachIndex([&](size_t i, size_t j) {
        is >> m_pMat[i][j];
    });
    return is;
}

template <typename T>
template <typename Func, typename... Args>
/**
 * @brief Aplica una funcion a todos los elementos.
 */
void Matrix1<T>::ApplyFunctionToAll(Func func, Args&& ...args) {
    ForEachIndex([&](size_t i, size_t j) {
        func(m_pMat[i][j], std::forward<Args>(args)...);
    });
}

template <typename T>
/**
 * @brief Imprime dimensiones y contenido de la matriz.
 */
ostream &Matrix1<T>::Print(ostream &os) {
    os << "Dimensiones: ";
    os << m_rows << " x " << m_cols << "\n";
    ForEachIndex([&](size_t i, size_t j) {
        os << m_pMat[i][j] << " ";
        if (j + 1 == m_cols)
            os << endl;
    });
    return os;
}

template <typename T>
/**
 * @brief Libera filas, arreglo principal y reinicia dimensiones.
 */
void Matrix1<T>::Destroy() {
    if (m_pMat != nullptr) {
        for (size_t i = 0; i < m_rows; ++i)
            delete[] m_pMat[i];
        delete[] m_pMat;
        m_pMat = nullptr;
    }
    m_rows = 0;
    m_cols = 0;
}

template <typename T>
/**
 * @brief Operador de lectura por stream.
 */
istream &operator>>(istream &is, Matrix1<T> &matrix) {
    return matrix.Read(is);
}

template <typename T>
/**
 * @brief Operador de impresion por stream.
 */
ostream &operator<<(ostream &os, Matrix1<T> &matrix) {
    return matrix.Print(os);
}

// Copy assignment operator
template <typename T>
/**
 * @brief Asigna copiando profundamente otra matriz.
 */
Matrix1<T> &Matrix1<T>::operator=(const Matrix1<T> &other) {
    if (this != &other) {
        Destroy();

        m_rows = other.m_rows;
        m_cols = other.m_cols;
       
        if(other.m_pMat != nullptr) {
            Create();

            ForEachIndex([&](size_t i, size_t j) {
                m_pMat[i][j] = other.m_pMat[i][j];
            });
        }
    }
    return *this;
}

// Move assignment operator
template <typename T>
/**
 * @brief Asigna moviendo los recursos de otra matriz.
 */
Matrix1<T> &Matrix1<T>::operator=(Matrix1<T> &&other) noexcept {
    if (this != &other) {
        Destroy();

        m_pMat = std::exchange(other.m_pMat, nullptr);
        m_rows = std::exchange(other.m_rows, 0);
        m_cols = std::exchange(other.m_cols, 0);
    }
    return *this;
}

// Operator +
template <typename T>
/**
 * @brief Suma elemento a elemento dos matrices.
 */
Matrix1<T> Matrix1<T>::operator+(const Matrix1<T> &other) const {
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Matrices must have the same dimensions for addition.");
    
    Matrix1<T> result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] + other.m_pMat[i][j];
    });

    return result;
}

// Operator -
template <typename T>
/**
 * @brief Resta elemento a elemento dos matrices.
 */
Matrix1<T> Matrix1<T>::operator-(const Matrix1 &other) const {
    if(m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");

    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] - other.m_pMat[i][j];
    });
    return result;
}

// Operator(const Matrix1<T> &other)* -- Multiplica dos matrices
template <typename T>
/**
 * @brief Multiplica dos matrices mediante filas por columnas.
 */
Matrix1<T> Matrix1<T>::operator*(const Matrix1 &other) const {
    if(m_cols != other.m_rows)
        throw std::invalid_argument("Columnas de la primera matriz deben ser iguales a filas de la segunda matriz para multiplicación.");

    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = other.m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = T(); // Initialize to zero
        for (size_t k = 0; k < m_cols; ++k)
            result.m_pMat[i][j] += m_pMat[i][k] * other.m_pMat[k][j];
    });
    return result;
}

// Operator(T value)* -- Multiplica cada elemento de la matriz por un valor escalar - Operador miembro
template <typename T>
/**
 * @brief Multiplica cada celda de la matriz por un escalar.
 */
Matrix1<T> Matrix1<T>::operator*(T value) const {
    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[i][j] * value;
    });

    return result;
}

template <typename T>
/**
 * @brief Construye la matriz transpuesta.
 */
Matrix1<T> Matrix1<T>::Transpose() const {
    Matrix1 result;
    result.m_rows = m_cols;
    result.m_cols = m_rows;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = m_pMat[j][i];
    });

    return result;
}

template <typename T>
/**
 * @brief Aplica una funcion a cada elemento y devuelve una matriz nueva.
 */
Matrix1<T> Matrix1<T>::Apply(function<T(const T&)> func) const {
    Matrix1 result;
    result.m_rows = m_rows;
    result.m_cols = m_cols;
    result.Create();

    result.ForEachIndex([&](size_t i, size_t j) {
        result.m_pMat[i][j] = func(m_pMat[i][j]);
    });

    return result;
}

template <typename T>
/**
 * @brief Suma los elementos de la diagonal principal.
 */
T Matrix1<T>::Trace() const {
    if (m_rows != m_cols)
        throw std::invalid_argument("La traza requiere una matriz cuadrada.");

    T result = T();
    for (size_t i = 0; i < m_rows; ++i)
        result += m_pMat[i][i];

    return result;
}

// Operator(T value, const Matrix1<T> &matrix) -- Multiplica escalar por matriz cuando el escalar está a la izquierda
template <typename T>
/**
 * @brief Permite multiplicacion escalar cuando el escalar esta a la izquierda.
 */
Matrix1<T> operator*(T value, const Matrix1<T> &matrix) {
    return matrix * value; // Reutiliza la implementación de multiplicación escalar
}

#endif // __MATRIX_H__
