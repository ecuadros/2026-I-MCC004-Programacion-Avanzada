"""
Pruebas de todas las operaciones del modulo matrix1.

Antes de correr, compila el modulo:
    python3 setup.py build_ext --inplace
Luego:
    python3 test_operaciones.py

Cada bloque imprime lo que hace y usa assert para verificar el resultado.
Si algo falla, el assert corta la ejecucion y muestra el error.
"""

import matrix1


def titulo(texto):
    print("\n" + "=" * 60)
    print(texto)
    print("=" * 60)


# =====================================================================
# REQUISITO 1: hacer posible  m[3][2] = 8
# =====================================================================
titulo("REQUISITO 1:  m[3][2] = 8")

m = matrix1.Matrix1(4, 3, 0.0)   # 4 filas x 3 columnas, llena de 0
m[3][2] = 8                      # escritura por doble corchete (memoryview)
assert m[3, 2] == 8              # se confirma leyendo con m[i, j]
print("m[3][2] = 8  ->  m[3,2] =", m[3, 2])

# tambien se puede escribir/leer con m[i, j]
m[0, 0] = 5
assert m[0, 0] == 5
print("m[0,0] = 5  ->  m[0,0] =", m[0, 0])


# =====================================================================
# REQUISITO 2: print(m)
# =====================================================================
titulo("REQUISITO 2:  print(m)")

print(m)


# =====================================================================
# REQUISITO 3: m1 = m2 * m3  (multiplicacion de matrices)
# =====================================================================
titulo("REQUISITO 3:  m1 = m2 * m3")

m2 = matrix1.Matrix1(2, 3, 1.0)  # 2x3 llena de 1
m3 = matrix1.Matrix1(3, 2, 2.0)  # 3x2 llena de 2
m1 = m2 * m3                     # resultado 2x2, cada elemento = 1*2 * 3 = 6
print("m2 (2x3):")
print(m2)
print("m3 (3x2):")
print(m3)
print("m1 = m2 * m3 (2x2):")
print(m1)
assert m1[0, 0] == 6.0
assert m1.rows() == 2 and m1.cols() == 2


# =====================================================================
# OPERADORES MATRIZ-MATRIZ:  + , - , +=, -=
# =====================================================================
titulo("OPERADORES MATRIZ-MATRIZ:  + - += -=")

a = matrix1.Matrix1(2, 2, 3.0)   # todo 3
b = matrix1.Matrix1(2, 2, 1.0)   # todo 1

suma = a + b                     # todo 4
resta = a - b                    # todo 2
assert suma[0, 0] == 4.0
assert resta[0, 0] == 2.0
print("a + b [0,0] =", suma[0, 0], " (esperado 4)")
print("a - b [0,0] =", resta[0, 0], " (esperado 2)")

a += b                           # a pasa a ser todo 4
assert a[0, 0] == 4.0
print("a += b -> a[0,0] =", a[0, 0], " (esperado 4)")


# =====================================================================
# OPERADORES MATRIZ-ESCALAR:  m + s , s * m , etc.
# =====================================================================
titulo("OPERADORES MATRIZ-ESCALAR")

c = matrix1.Matrix1(2, 2, 2.0)   # todo 2
mas = c + 10.0                   # todo 12
por = c * 3.0                    # todo 6
por_izq = 3.0 * c                # todo 6 (escalar por la izquierda)
assert mas[0, 0] == 12.0
assert por[0, 0] == 6.0
assert por_izq[0, 0] == 6.0
print("c + 10  [0,0] =", mas[0, 0], " (esperado 12)")
print("c * 3   [0,0] =", por[0, 0], " (esperado 6)")
print("3 * c   [0,0] =", por_izq[0, 0], " (esperado 6, escalar a la izquierda)")


# =====================================================================
# COMPARACION:  ==  y  !=
# =====================================================================
titulo("COMPARACION:  ==  !=")

x = matrix1.Matrix1(2, 2, 7.0)
y = matrix1.Matrix1(2, 2, 7.0)
z = matrix1.Matrix1(2, 2, 9.0)
assert (x == y) is True
assert (x != z) is True
print("x == y :", x == y, " (esperado True)")
print("x != z :", x != z, " (esperado True)")


# =====================================================================
# MULTIPLICACION ELEMENTO A ELEMENTO (Hadamard)
# =====================================================================
titulo("ELEMENTO A ELEMENTO:  element_wise_multiply")

p = matrix1.Matrix1(2, 2, 4.0)
q = matrix1.Matrix1(2, 2, 5.0)
had = p.element_wise_multiply(q)  # cada elemento = 4 * 5 = 20
assert had[0, 0] == 20.0
print("p .* q [0,0] =", had[0, 0], " (esperado 20)")


# =====================================================================
# MULTIPLICACION CONCURRENTE (std::thread) == misma que la normal
# =====================================================================
titulo("MULTIPLICACION CONCURRENTE:  multiply_concurrent")

normal = m2 * m3                       # 2x2 lleno de 6
conc = m2.multiply_concurrent(m3)      # nthreads=0 usa los nucleos disponibles
assert normal == conc                  # deben dar el mismo resultado
print("m2 * m3 (normal) == m2.multiply_concurrent(m3) :", normal == conc)


# =====================================================================
# MANEJO DE ERRORES: dimensiones incompatibles -> excepcion
# =====================================================================
titulo("MANEJO DE ERRORES")

try:
    matrix1.Matrix1(2, 2, 1.0) + matrix1.Matrix1(3, 3, 1.0)
    assert False, "deberia haber lanzado excepcion"
except (ValueError, RuntimeError) as e:
    print("suma 2x2 + 3x3 -> excepcion:", e)

try:
    matrix1.Matrix1(2, 3, 1.0) * matrix1.Matrix1(2, 2, 1.0)
    assert False, "deberia haber lanzado excepcion"
except (ValueError, RuntimeError) as e:
    print("mult 2x3 * 2x2 -> excepcion:", e)


# =====================================================================
titulo("OK: todas las operaciones funcionan")
