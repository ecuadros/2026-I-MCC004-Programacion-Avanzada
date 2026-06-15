from matrix1_cpp import Matrix


## @file matrix1CppDemo.py
## @brief Demos en Python usando la clase Matrix1 de C++ expuesta con pybind11.
##
## Este archivo no implementa la matriz. Importa la clase Matrix desde el modulo
## compilado matrix1_cpp y demuestra que Python puede usar la clase C++.


## @brief Demuestra la indexacion doble desde Python.
##
## Crea una matriz C++ de 4 x 3, asigna el valor 8 en la posicion m[3][2]
## y luego imprime la matriz usando el metodo __str__ expuesto por pybind11.
def demo_indexing():
    print("Demo C++ desde Python: m[3][2] = 8")
    m = Matrix(4, 3)
    m[3][2] = 8
    print(m)


## @brief Demuestra la multiplicacion de dos matrices C++ desde Python.
##
## Crea una matriz de 2 x 3 y otra de 3 x 2. Luego ejecuta m1 = m2 * m3,
## lo cual llama internamente al operator* definido en la clase C++ Matrix1.
def demo_matrix_multiplication():
    print("\nDemo C++ desde Python: m1 = m2 * m3")
    m2 = Matrix(2, 3)
    m3 = Matrix(3, 2)

    values_m2 = [[1, 2, 3], [4, 5, 6]]
    values_m3 = [[1, 2], [3, 4], [5, 6]]

    for i, row in enumerate(values_m2):
        for j, value in enumerate(row):
            m2[i][j] = value

    for i, row in enumerate(values_m3):
        for j, value in enumerate(row):
            m3[i][j] = value

    m1 = m2 * m3
    print(m1)


## @brief Demuestra una expresion compuesta similar al demo de C++.
##
## La expresion m1 = 5 * m2 + m3 * m4 prueba tres enlaces a la vez:
## multiplicacion escalar por la izquierda, multiplicacion matricial y suma
## de matrices.
def demo_cpp_expression():
    print("\nDemo C++ desde Python: m1 = 5 * m2 + m3 * m4")
    m2 = Matrix(2, 2)
    m3 = Matrix(2, 3)
    m4 = Matrix(3, 2)

    values_m2 = [[1, 2], [3, 4]]
    values_m3 = [[1, 2, 3], [4, 5, 6]]
    values_m4 = [[1, 2], [3, 4], [5, 6]]

    for i, row in enumerate(values_m2):
        for j, value in enumerate(row):
            m2[i][j] = value

    for i, row in enumerate(values_m3):
        for j, value in enumerate(row):
            m3[i][j] = value

    for i, row in enumerate(values_m4):
        for j, value in enumerate(row):
            m4[i][j] = value

    m1 = 5 * m2 + m3 * m4
    print(m1)


## @brief Demuestra los tres extras expuestos desde C++ hacia Python.
##
## Prueba transpose(), apply() y trace() usando el modulo compilado con
## pybind11. La matriz sigue siendo una Matrix1<int> de C++.
def demo_extras():
    print("\nDemo extras C++ desde Python")
    m = Matrix(2, 2)
    values = [[1, 2], [3, 4]]

    for i, row in enumerate(values):
        for j, value in enumerate(row):
            m[i][j] = value

    print("Extra #1 transpose:")
    print(m.transpose())

    print("Extra #2 apply(lambda x: x + 1):")
    print(m.apply(lambda x: x + 1))

    print("Extra #2 usando Apply de C++ con incremento 10:")
    print(m.extra_apply_cpp(10))

    print("Extra #3 trace:")
    print(m.trace())


## @brief Ejecuta todos los demos del archivo.
def main():
    demo_indexing()
    demo_matrix_multiplication()
    demo_cpp_expression()
    demo_extras()


if __name__ == "__main__":
    main()
