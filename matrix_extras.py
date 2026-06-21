"""
Tres extras para Matrix1, escritos como funciones de Python SOBRE el
modulo matrix1 ya compilado. No tocan el codigo C++: solo usan lo que
ya esta expuesto (Matrix1(r,c,v), m[i,j], rows(), cols()).

Extras:
    1. identity(n)   -> matriz identidad n x n
    2. transpose(m)  -> matriz transpuesta
    3. trace(m)      -> traza (suma de la diagonal principal)
"""

import matrix1


def identity(n):
    """Extra 1: matriz identidad n x n (1 en la diagonal, 0 en el resto)."""
    m = matrix1.Matrix1(n, n, 0.0)
    for i in range(n):
        m[i, i] = 1.0
    return m


def transpose(m):
    """Extra 2: transpuesta. El elemento (i, j) pasa a la posicion (j, i)."""
    rows, cols = m.rows(), m.cols()
    t = matrix1.Matrix1(cols, rows, 0.0)
    for i in range(rows):
        for j in range(cols):
            t[j, i] = m[i, j]
    return t


def trace(m):
    """Extra 3: traza = suma de la diagonal. Solo para matrices cuadradas."""
    if m.rows() != m.cols():
        raise ValueError("La traza solo esta definida para matrices cuadradas")
    return sum(m[i, i] for i in range(m.rows()))


if __name__ == "__main__":
    I = identity(3)
    print("identity(3):")
    print(I)

    a = matrix1.Matrix1(2, 3, 0.0)
    a[0, 0] = 1; a[0, 1] = 2; a[0, 2] = 3
    a[1, 0] = 4; a[1, 1] = 5; a[1, 2] = 6
    print("A:")
    print(a)
    print("transpose(A):")
    print(transpose(a))

    print("trace(identity(3)) =", trace(I))
