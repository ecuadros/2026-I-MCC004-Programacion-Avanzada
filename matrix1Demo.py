from matrix1 import Matrix

## @brief Demo para m[3][2] = 8 y print(m).
def demo_python_indexing():
    print("Demo Python: m[3][2] = 8")
    matrix = Matrix(4, 3)
    matrix[3][2] = 8
    print(matrix)


## @brief Demo para m1 = m2 * m3.
def demo_matrix_multiplication():
    print("\nDemo: m1 = m2 * m3")
    m2 = Matrix.from_rows([[1, 2, 3], [4, 5, 6]])
    m3 = Matrix.from_rows([[1, 2], [3, 4], [5, 6]])
    m1 = m2 * m3
    print(m1)


## @brief Demo de los tres extras.
def demo_extras():
    print("\nDemo extras")
    matrix = Matrix.from_rows([[1, 2], [3, 4]])

    print("Extra #1 Transpose:")
    print(matrix.transpose())

    print("Extra #2 Apply(lambda x: x + 1):")
    print(matrix.apply(lambda x: x + 1))

    print("Extra #3 Trace:")
    print(matrix.trace())


## @brief Demo de threads.
def demo_threads():
    print("\nDemo concurrencia con Threads")

    m2 = Matrix.from_rows([[1, 2, 3], [4, 5, 6]])
    m3 = Matrix.from_rows([[1, 2], [3, 4], [5, 6]])

    m1 = m2.multiply_threaded(m3)

    print(m1)


## @brief DemoCPP con expresion similar al codigo C++.
def demo_cpp():
    print("\nDemoCPP: m1 = 5 * m2 + m3 * m4")
    m2 = Matrix.from_rows([[1, 2], [3, 4]])
    m3 = Matrix.from_rows([[1, 2, 3], [4, 5, 6]])
    m4 = Matrix.from_rows([[1, 2], [3, 4], [5, 6]])

    m1 = 5 * m2 + m3 * m4
    print(m1)


def main():
    demo_python_indexing()
    demo_matrix_multiplication()
    demo_extras()
    demo_threads()
    demo_cpp()


if __name__ == "__main__":
    main()