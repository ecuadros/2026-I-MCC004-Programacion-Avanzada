from matrix_py import MatrixInt, MatrixFloat, crear_matriz

print("Matriz de enteros:")

m = MatrixInt(2, 2)
m.set(0, 0, 5)
m.set(0, 1, 6)
m.set(1, 0, 7)
m.set(1, 1, 8)

print(m)

print("Matriz de flotantes:")

f = MatrixFloat(2, 2)
f.set(0, 0, 1.5)
f.set(0, 1, 2.5)
f.set(1, 0, 3.5)
f.set(1, 1, 4.5)

print(f)

print("Matriz creada con parametro tipo:")

x = crear_matriz("float", 2, 2)
x.set(0, 0, 10.5)
x.set(0, 1, 20.5)
x.set(1, 0, 30.5)
x.set(1, 1, 40.5)

print(x)