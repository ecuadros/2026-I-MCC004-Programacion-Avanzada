"""
Pruebas de todas las operaciones del modulo matrix1, para AMBOS tipos.
 
El modulo expone:
    matrix1.Matrix1Int    -> Matrix1<int>
    matrix1.Matrix1Float  -> Matrix1<float>
 
La bateria completa se corre una vez por tipo. El parametro T es el tipo
nativo de Python (int / float) que se usa para construir los valores
esperados, de modo que las comparaciones sean del tipo correcto.
 
Uso:
    python3 setup.py build_ext --inplace
    python3 test_operaciones.py
"""
 
import matrix1
 
 
def titulo(texto):
    print("\n" + "=" * 60)
    print(texto)
    print("=" * 60)
 
 
def suite(Matrix, nombre, T):
    titulo("TIPO: " + nombre)
 
    # --- REQUISITO 1: m[3][2] = 8 ---
    m = Matrix(4, 3)
    m[3][2] = T(8)
    assert m[3, 2] == T(8)
    m[0, 0] = T(5)
    assert m[0, 0] == T(5)
    print("m[3][2] = 8  ->  m[3,2] =", m[3, 2])
 
    # --- REQUISITO 2: print(m) ---
    print("print(m):")
    print(m)
 
    # --- REQUISITO 3: m1 = m2 * m3 ---
    m2 = Matrix(2, 3, T(1))
    m3 = Matrix(3, 2, T(2))
    m1 = m2 * m3                     # cada elemento = 3 * 1 * 2 = 6
    print("m1 = m2 * m3:")
    print(m1)
    assert m1[0, 0] == T(6)
    assert m1.rows() == 2 and m1.cols() == 2
 
    # --- OPERADORES MATRIZ-MATRIZ: + - += ---
    a = Matrix(2, 2, T(3))
    b = Matrix(2, 2, T(1))
    assert (a + b)[0, 0] == T(4)
    assert (a - b)[0, 0] == T(2)
    a += b
    assert a[0, 0] == T(4)
    print("+ - +=  OK")
 
    # --- OPERADORES MATRIZ-ESCALAR (incluye escalar por la izquierda) ---
    c = Matrix(2, 2, T(2))
    assert (c + T(10))[0, 0] == T(12)
    assert (c * T(3))[0, 0] == T(6)
    assert (T(3) * c)[0, 0] == T(6)
    print("m+s, m*s, s*m  OK")
 
    # --- COMPARACION == != ---
    x = Matrix(2, 2, T(7))
    y = Matrix(2, 2, T(7))
    z = Matrix(2, 2, T(9))
    assert (x == y) is True
    assert (x != z) is True
    print("== !=  OK")
 
    # --- ELEMENTO A ELEMENTO ---
    p = Matrix(2, 2, T(4))
    q = Matrix(2, 2, T(5))
    assert p.element_wise_multiply(q)[0, 0] == T(20)
    print("element_wise_multiply  OK")
 
    # --- MULTIPLICACION CONCURRENTE (== a la normal) ---
    assert (m2 * m3) == m2.multiply_concurrent(m3)
    print("multiply_concurrent  OK")
 
    # --- EXTRAS: identity / transpose / trace ---
    I = Matrix.identity(3)
    assert I[0, 0] == T(1) and I[0, 1] == T(0)
    t = Matrix(2, 3)
    t[0, 0] = T(1); t[0, 1] = T(2); t[0, 2] = T(3)
    t[1, 0] = T(4); t[1, 1] = T(5); t[1, 2] = T(6)
    tt = t.transpose()
    assert tt.rows() == 3 and tt.cols() == 2
    assert tt[0, 1] == T(4)            # el (1,0) original pasa a (0,1)
    assert I.trace() == T(3)
    print("identity / transpose / trace  OK")
 
    # --- MANEJO DE ERRORES ---
    try:
        Matrix(2, 2, T(1)) + Matrix(3, 3, T(1))
        assert False, "deberia haber lanzado excepcion"
    except (ValueError, RuntimeError) as e:
        print("suma 2x2 + 3x3 -> excepcion:", e)
    try:
        Matrix(2, 3, T(1)) * Matrix(2, 2, T(1))
        assert False, "deberia haber lanzado excepcion"
    except (ValueError, RuntimeError) as e:
        print("mult 2x3 * 2x2 -> excepcion:", e)
 
    print(">>> TODO OK para", nombre)
 
 
def suite_float_decimales():
    """Prueba especifica: Matrix1Float debe conservar valores con decimales."""
    titulo("Matrix1Float: datos con DECIMALES")
 
    m = matrix1.Matrix1Float(2, 2)
    m[0, 0] = 1.5
    m[0, 1] = 2.25
    m[1, 0] = 3.75
    m[1, 1] = 0.5
    print("m con decimales:")
    print(m)
 
    # 1.5, 2.25, 3.75 y 0.5 son exactos en float (potencias de 2),
    # asi que se pueden comparar con igualdad estricta.
    assert m[0, 0] == 1.5
    assert m[0, 1] == 2.25
    assert m[1, 0] == 3.75
    assert m[1, 1] == 0.5
    print("lectura de decimales  OK")
 
    # Escalar por decimal: cada valor se duplica.
    doble = m * 2.0
    assert doble[0, 0] == 3.0
    assert doble[0, 1] == 4.5
    assert doble[1, 0] == 7.5
    assert doble[1, 1] == 1.0
    print("m * 2.0 (decimales) [0,1] =", doble[0, 1], " (esperado 4.5)")
 
    # Suma de dos matrices con decimales.
    a = matrix1.Matrix1Float(2, 2, 0.25)
    b = matrix1.Matrix1Float(2, 2, 0.75)
    suma = a + b
    assert suma[0, 0] == 1.0
    print("0.25 + 0.75 =", suma[0, 0], " (esperado 1.0)")
 
    print(">>> TODO OK para decimales en Matrix1Float")
 
 
def main():
    suite(matrix1.Matrix1Int,   "Matrix1Int",   int)
    suite(matrix1.Matrix1Float, "Matrix1Float", float)
    suite_float_decimales()
    titulo("OK: todas las operaciones funcionan para int y float")
 
 
if __name__ == "__main__":
    main()
