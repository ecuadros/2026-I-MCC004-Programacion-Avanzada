"""
Pruebas de todas las operaciones del modulo matrix1, escritas de forma
directa (sin funciones), primero para Matrix1Int y luego para Matrix1Float.
 
Uso:
    python3 setup.py build_ext --inplace
    python3 test_operaciones.py
"""
 
import matrix1
 
# =====================================================================
#  MATRIX1INT  (Matrix1<int>)
# =====================================================================
print("=" * 60)
print("TIPO: Matrix1Int")
print("=" * 60)
 
# --- REQUISITO 1: m[3][2] = 8 ---
m = matrix1.Matrix1Int(4, 3)
m[3][2] = 8
assert m[3, 2] == 8
m[0, 0] = 5
assert m[0, 0] == 5
print("m[3][2] = 8  ->  m[3,2] =", m[3, 2])
 
# --- REQUISITO 2: print(m) ---
print("print(m):")
print(m)
 
# --- REQUISITO 3: m1 = m2 * m3 ---
m2 = matrix1.Matrix1Int(2, 3, 1)
m3 = matrix1.Matrix1Int(3, 2, 2)
m1 = m2 * m3
print("m1 = m2 * m3:")
print(m1)
assert m1[0, 0] == 6
assert m1.rows() == 2 and m1.cols() == 2
 
# --- MATRIZ-MATRIZ: + - += ---
a = matrix1.Matrix1Int(2, 2, 3)
b = matrix1.Matrix1Int(2, 2, 1)
assert (a + b)[0, 0] == 4
assert (a - b)[0, 0] == 2
a += b
assert a[0, 0] == 4
print("+ - +=  OK")
 
# --- MATRIZ-ESCALAR (incluye escalar por la izquierda) ---
c = matrix1.Matrix1Int(2, 2, 2)
assert (c + 10)[0, 0] == 12
assert (c * 3)[0, 0] == 6
assert (3 * c)[0, 0] == 6
print("m+s, m*s, s*m  OK")
 
# --- COMPARACION == != ---
x = matrix1.Matrix1Int(2, 2, 7)
y = matrix1.Matrix1Int(2, 2, 7)
z = matrix1.Matrix1Int(2, 2, 9)
assert (x == y) is True
assert (x != z) is True
print("== !=  OK")
 
# --- ELEMENTO A ELEMENTO ---
p = matrix1.Matrix1Int(2, 2, 4)
q = matrix1.Matrix1Int(2, 2, 5)
assert p.element_wise_multiply(q)[0, 0] == 20
print("element_wise_multiply  OK")
 
# --- CONCURRENTE (== a la normal) ---
assert (m2 * m3) == m2.multiply_concurrent(m3)
print("multiply_concurrent  OK")
 
# --- EXTRAS: identity / transpose / trace ---
I = matrix1.Matrix1Int.identity(3)
assert I[0, 0] == 1 and I[0, 1] == 0
t = matrix1.Matrix1Int(2, 3)
t[0, 0] = 1; t[0, 1] = 2; t[0, 2] = 3
t[1, 0] = 4; t[1, 1] = 5; t[1, 2] = 6
tt = t.transpose()
assert tt.rows() == 3 and tt.cols() == 2
assert tt[0, 1] == 4
assert I.trace() == 3
print("identity / transpose / trace  OK")
 
# --- ERRORES ---
try:
    matrix1.Matrix1Int(2, 2, 1) + matrix1.Matrix1Int(3, 3, 1)
    assert False
except (ValueError, RuntimeError) as e:
    print("suma 2x2 + 3x3 -> excepcion:", e)
try:
    matrix1.Matrix1Int(2, 3, 1) * matrix1.Matrix1Int(2, 2, 1)
    assert False
except (ValueError, RuntimeError) as e:
    print("mult 2x3 * 2x2 -> excepcion:", e)
 
 
# =====================================================================
#  MATRIX1FLOAT  (Matrix1<float>)
# =====================================================================
print("\n" + "=" * 60)
print("TIPO: Matrix1Float")
print("=" * 60)
 
# --- REQUISITO 1: m[3][2] = 8 ---
m = matrix1.Matrix1Float(4, 3)
m[3][2] = 8.0
assert m[3, 2] == 8.0
m[0, 0] = 5.0
assert m[0, 0] == 5.0
print("m[3][2] = 8  ->  m[3,2] =", m[3, 2])
 
# --- REQUISITO 2: print(m) ---
print("print(m):")
print(m)
 
# --- REQUISITO 3: m1 = m2 * m3 ---
m2 = matrix1.Matrix1Float(2, 3, 1.0)
m3 = matrix1.Matrix1Float(3, 2, 2.0)
m1 = m2 * m3
print("m1 = m2 * m3:")
print(m1)
assert m1[0, 0] == 6.0
assert m1.rows() == 2 and m1.cols() == 2
 
# --- MATRIZ-MATRIZ: + - += ---
a = matrix1.Matrix1Float(2, 2, 3.0)
b = matrix1.Matrix1Float(2, 2, 1.0)
assert (a + b)[0, 0] == 4.0
assert (a - b)[0, 0] == 2.0
a += b
assert a[0, 0] == 4.0
print("+ - +=  OK")
 
# --- MATRIZ-ESCALAR (incluye escalar por la izquierda) ---
c = matrix1.Matrix1Float(2, 2, 2.0)
assert (c + 10.0)[0, 0] == 12.0
assert (c * 3.0)[0, 0] == 6.0
assert (3.0 * c)[0, 0] == 6.0
print("m+s, m*s, s*m  OK")
 
# --- COMPARACION == != ---
x = matrix1.Matrix1Float(2, 2, 7.0)
y = matrix1.Matrix1Float(2, 2, 7.0)
z = matrix1.Matrix1Float(2, 2, 9.0)
assert (x == y) is True
assert (x != z) is True
print("== !=  OK")
 
# --- ELEMENTO A ELEMENTO ---
p = matrix1.Matrix1Float(2, 2, 4.0)
q = matrix1.Matrix1Float(2, 2, 5.0)
assert p.element_wise_multiply(q)[0, 0] == 20.0
print("element_wise_multiply  OK")
 
# --- CONCURRENTE (== a la normal) ---
assert (m2 * m3) == m2.multiply_concurrent(m3)
print("multiply_concurrent  OK")
 
# --- EXTRAS: identity / transpose / trace ---
I = matrix1.Matrix1Float.identity(3)
assert I[0, 0] == 1.0 and I[0, 1] == 0.0
t = matrix1.Matrix1Float(2, 3)
t[0, 0] = 1.0; t[0, 1] = 2.0; t[0, 2] = 3.0
t[1, 0] = 4.0; t[1, 1] = 5.0; t[1, 2] = 6.0
tt = t.transpose()
assert tt.rows() == 3 and tt.cols() == 2
assert tt[0, 1] == 4.0
assert I.trace() == 3.0
print("identity / transpose / trace  OK")
 
# --- ERRORES ---
try:
    matrix1.Matrix1Float(2, 2, 1.0) + matrix1.Matrix1Float(3, 3, 1.0)
    assert False
except (ValueError, RuntimeError) as e:
    print("suma 2x2 + 3x3 -> excepcion:", e)
try:
    matrix1.Matrix1Float(2, 3, 1.0) * matrix1.Matrix1Float(2, 2, 1.0)
    assert False
except (ValueError, RuntimeError) as e:
    print("mult 2x3 * 2x2 -> excepcion:", e)
 
 
# =====================================================================
#  MATRIX1FLOAT con DECIMALES (que no se pierdan)
# =====================================================================
print("\n" + "=" * 60)
print("Matrix1Float: datos con DECIMALES")
print("=" * 60)
 
m = matrix1.Matrix1Float(2, 2)
m[0, 0] = 1.5
m[0, 1] = 2.25
m[1, 0] = 3.75
m[1, 1] = 0.5
print("m con decimales:")
print(m)
 
# 1.5, 2.25, 3.75 y 0.5 son exactos en float, se comparan con ==
assert m[0, 0] == 1.5
assert m[0, 1] == 2.25
assert m[1, 0] == 3.75
assert m[1, 1] == 0.5
print("lectura de decimales  OK")
 
doble = m * 2.0
assert doble[0, 0] == 3.0
assert doble[0, 1] == 4.5
assert doble[1, 0] == 7.5
assert doble[1, 1] == 1.0
print("m * 2.0 (decimales) [0,1] =", doble[0, 1], " (esperado 4.5)")
 
a = matrix1.Matrix1Float(2, 2, 0.25)
b = matrix1.Matrix1Float(2, 2, 0.75)
suma = a + b
assert suma[0, 0] == 1.0
print("0.25 + 0.75 =", suma[0, 0], " (esperado 1.0)")
 
 
print("\n" + "=" * 60)
print("OK: todas las operaciones funcionan para int y float")
print("=" * 60)
 