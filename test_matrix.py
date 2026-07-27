from matrix_py import Matrix, MatrixDecimal

print("=== Demo 1: hacer posible m[3][2] = 8 ===")

m = Matrix(4, 3)
m[3][2] = 8

print(m)

print("=== Demo 2: m1 = m2 * m3 ===")

m2 = Matrix(2, 3)
m3 = Matrix(3, 2)

valores_m2 = [
    [1, 2, 3],
    [4, 5, 6]
]

valores_m3 = [
    [1, 2],
    [3, 4],
    [5, 6]
]

for i in range(2):
    for j in range(3):
        m2[i][j] = valores_m2[i][j]

for i in range(3):
    for j in range(2):
        m3[i][j] = valores_m3[i][j]

m1 = m2 * m3

print(m1)

print("=== Extra #1: transpuesta ===")
print(m2.transpose())

print("=== Extra #2: traza ===")

cuadrada = Matrix(2, 2)
cuadrada[0][0] = 1
cuadrada[0][1] = 2
cuadrada[1][0] = 3
cuadrada[1][1] = 4

print(cuadrada.trace())

print("=== Extra #3: sumar un valor a toda la matriz ===")
print(cuadrada.add_value(10))

print("=== Demo con matriz decimal ===")

f = MatrixDecimal(2, 2)
f[0][0] = 1.5
f[0][1] = 2.5
f[1][0] = 3.5
f[1][1] = 4.5

print(f)