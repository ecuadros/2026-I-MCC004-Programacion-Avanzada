"""
Tres extras para Matrix1, escritos como funciones de Python SOBRE el
modulo matrix1 ya compilado. No tocan el codigo C++: solo usan lo que
ya esta expuesto (Matrix(r, c, v), m[i, j], rows(), cols()).
 
Como el modulo expone dos tipos (Matrix1Int y Matrix1Float), cada funcion
recibe la CLASE a usar, de modo que sirven con cualquiera de los dos.
 
Extras:
    1. identity(Matrix, n)    -> matriz identidad n x n
    2. transpose(Matrix, m)   -> matriz transpuesta
    3. trace(m)               -> traza (suma de la diagonal principal)
"""
 
import matrix1
 
 
def identity(Matrix, n):
    """Extra 1: matriz identidad n x n (1 en la diagonal, 0 en el resto)."""
    m = Matrix(n, n)              # queda en 0
    for i in range(n):
        m[i, i] = 1
    return m
 
 
def transpose(Matrix, m):
    """Extra 2: transpuesta. El elemento (i, j) pasa a la posicion (j, i)."""
    rows, cols = m.rows(), m.cols()
    t = Matrix(cols, rows)
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
    for nombre, Matrix in [("Int", matrix1.Matrix1Int),
                           ("Float", matrix1.Matrix1Float)]:
        print("\n====  ", nombre, "  ====")
 
        I = identity(Matrix, 3)
        print("identity(3):")
        print(I)
 
        a = Matrix(2, 3)
        a[0, 0] = 1; a[0, 1] = 2; a[0, 2] = 3
        a[1, 0] = 4; a[1, 1] = 5; a[1, 2] = 6
        print("A:")
        print(a)
        print("transpose(A):")
        print(transpose(Matrix, a))
 
        print("trace(identity(3)) =", trace(I))