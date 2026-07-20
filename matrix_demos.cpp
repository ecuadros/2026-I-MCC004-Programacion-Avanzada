
// Demos de Expresiones Regulares y Threads sobre Matrix1<T>
// Compilar: se agrega matrix_demos.cpp al Makefile (ya usa -pthread)
 
#include <iostream>
#include <sstream>
#include <regex>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <vector>
#include "matrix1.h"
#include "matrix_demos.h"
#include "types.h"
 
using namespace std;
 
template <typename T>
void Square(T &n) { n *= n; }
 
/**
 * @brief Parsea un string con formato "NxM: n1 n2 ... / fila2 ..." usando std::regex.
 *
 * Primero busca las dimensiones NxM y luego extrae todos los numeros
 * que siguen. Con eso arma el formato "rows cols n1 n2 ..." que ya
 * entiende Matrix1::Read().
 *
 * @tparam T Tipo de los elementos (TI, T5, etc de types.h).
 * @param text String de entrada. Ej: "2x3: 1 2 3 / 4 5 6".
 * @return Matrix1<T> con los datos parseados (se retorna por move).
 */
template <typename T>
Matrix1<T> MatrixFromString(const string &text)
{   // (\d+) captura un grupo de digitos, [xX] acepta ambas formas
    regex dim_re(R"((\d+)\s*[xX]\s*(\d+))");
    smatch dim;
    BoolType ok = regex_search(text, dim, dim_re);
    assert(ok && "Formato invalido: se espera NxM");
 
    size_t rows = stoul(dim[1].str());
    size_t cols = stoul(dim[2].str());
 
    // Busca todos los numeros (enteros, con signo opcional) despues del NxM
    string rest = text.substr(dim.position(0) + dim.length(0));
    regex num_re(R"(-?\d+)");
 
    ostringstream oss;
    oss << rows << " " << cols;
    for(auto it = sregex_iterator(rest.begin(), rest.end(), num_re);
        it != sregex_iterator(); ++it)
        oss << " " << it->str();
 
    Matrix1<T> mat;
    istringstream iss(oss.str());
    iss >> mat;
    assert(mat.rows() == rows && mat.cols() == cols);
    return mat;
}
 
/**
 * @brief Demo: lee una matriz como texto libre y la parsea con std::regex.
 */
void DemoMatrixRegex()
{   cout << "=== DemoMatrixRegex ===\n";
    cout << "Ingrese la matriz (formato: NxM: n1 n2 ... / fila2 ...):\n";
 
    string linea;
    getline(cin, linea);
 
    Matrix1<TI> mat = MatrixFromString<TI>(linea);
    cout << "Matriz parseada:\n" << mat;
 
    mat.ApplyFunctionToAll(Square<TI>);
    cout << "Despues de Square:\n" << mat;
}
 
/**
 * @brief Multiplica C = A * B de forma secuencial (tres loops clasicos).
 *
 * Version de referencia: un solo hilo hace todo el trabajo.
 * Sirve para verificar y cronometrar contra la version paralela.
 *
 * @param A Matriz izquierda.
 * @param B Matriz derecha (B.rows() == A.cols()).
 * @param C Matriz resultado, ya inicializada con Init(A.rows(), B.cols()).
 */
static void MulSecuencial(const Matrix1<TI> &A, const Matrix1<TI> &B, Matrix1<TI> &C)
{   for(size_t i = 0 ; i < A.rows() ; ++i)
        for(size_t j = 0 ; j < B.cols() ; ++j)
        {   C[i][j] = TI{};
            for(size_t k = 0 ; k < A.cols() ; ++k)
                C[i][j] += A[i][k] * B[k][j];
        }
}
 
/**
 * @brief Multiplica C = A * B en paralelo, un jthread por fila de C.
 *
 * A y B solo se leen (compartir lecturas es seguro) y cada hilo escribe
 * unicamente en su propia fila de C, asi que no hay race conditions y
 * no hace falta mutex sobre los datos. Los jthread hacen join automatico
 * al salir del scope.
 *
 * @param A Matriz izquierda.
 * @param B Matriz derecha (B.rows() == A.cols()).
 * @param C Matriz resultado, ya inicializada con Init(A.rows(), B.cols()).
 */
static void MulParalelo(const Matrix1<TI> &A, const Matrix1<TI> &B, Matrix1<TI> &C)
{   vector<jthread> threads;
    for(size_t i = 0 ; i < A.rows() ; ++i)
        threads.emplace_back([&A, &B, &C, i]()
        {   for(size_t j = 0 ; j < B.cols() ; ++j)
            {   C[i][j] = TI{};
                for(size_t k = 0 ; k < A.cols() ; ++k)
                    C[i][j] += A[i][k] * B[k][j];
            }
        });
} // al cerrar el scope, cada jthread hace join solo
 
/**
 * @brief Demo: C = A * B calculado sin y con threads, comparando resultados y tiempos.
 *
 * Primero multiplica las matrices que ingresa el usuario, mostrando que
 * ambas versiones dan la misma C. Luego repite la comparacion con dos
 * matrices grandes generadas al azar para que la diferencia de tiempo
 * entre un hilo y un hilo-por-fila sea visible.
 */
void DemoMatrixThreadsMul()
{   cout << "=== DemoMatrixThreadsMul: C = A*B, secuencial vs paralelo ===\n";
 
    cout << "Ingrese A (formato: NxM: n1 n2 ... / fila2 ...):\n";
    string lineaA; getline(cin, lineaA);
    Matrix1<TI> A = MatrixFromString<TI>(lineaA);
 
    cout << "Ingrese B (filas de B = columnas de A):\n";
    string lineaB; getline(cin, lineaB);
    Matrix1<TI> B = MatrixFromString<TI>(lineaB);
 
    assert(A.cols() == B.rows() && "Dimensiones incompatibles");
 
    // ── 1. Con las matrices del usuario: verificar que dan lo mismo ──────
    Matrix1<TI> Cseq, Cpar;
    Cseq.Init(A.rows(), B.cols());
    Cpar.Init(A.rows(), B.cols());
 
    MulSecuencial(A, B, Cseq);
    MulParalelo(A, B, Cpar);
 
    cout << "A:\n" << A << "B:\n" << B;
    cout << "C secuencial (1 hilo):\n" << Cseq;
    cout << "C paralelo (" << A.rows() << " hilos, uno por fila):\n" << Cpar;
 
    BoolType iguales = true;
    for(size_t i = 0 ; iguales && i < Cseq.rows() ; ++i)
        for(size_t j = 0 ; iguales && j < Cseq.cols() ; ++j)
            iguales = (Cseq[i][j] == Cpar[i][j]);
    cout << "Verificacion: " << (iguales ? "IGUALES" : "DIFERENTES!") << "\n";
 
    // ── 2. Con matrices grandes: medir la diferencia de tiempo ──────────
    const size_t N = 600;   // 600x600: suficiente para que se note
    cout << "\nBenchmark con matrices " << N << "x" << N << " aleatorias"
         << " (nucleos disponibles: " << jthread::hardware_concurrency() << "):\n";
    // Nota: con 1 solo nucleo el paralelo puede salir MAS LENTO que el
    // secuencial (crear hilos cuesta y no hay donde correrlos en simultaneo).
    // El speedup real aparece con 2+ nucleos.
 
    Matrix1<TI> GA, GB, GC;
    GA.Init(N, N); GB.Init(N, N); GC.Init(N, N);
    srand(42);
    for(size_t i = 0 ; i < N ; ++i)
        for(size_t j = 0 ; j < N ; ++j)
        {   GA[i][j] = rand() % 10;
            GB[i][j] = rand() % 10;
        }
 
    auto t0 = chrono::steady_clock::now();
    MulSecuencial(GA, GB, GC);
    auto t1 = chrono::steady_clock::now();
    auto ms_seq = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    TI check_seq = GC[0][0] + GC[N-1][N-1];
 
    t0 = chrono::steady_clock::now();
    MulParalelo(GA, GB, GC);
    t1 = chrono::steady_clock::now();
    auto ms_par = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    TI check_par = GC[0][0] + GC[N-1][N-1];
 
    cout << "  Secuencial (1 hilo):     " << ms_seq << " ms\n";
    cout << "  Paralelo (" << N << " hilos):    " << ms_par << " ms\n";
    if(ms_par > 0)
        cout << "  Speedup: x" << (T5)ms_seq / ms_par << "\n";
    cout << "  Verificacion: " << (check_seq == check_par ? "IGUALES" : "DIFERENTES!") << "\n";
}