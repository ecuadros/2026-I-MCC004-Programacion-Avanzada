from matrix_wrapper import SimpleMatrix

# Cargar primera matriz
m = SimpleMatrix()
m.load("Datamatriz1.txt")
print("Matriz A:")
print(m)

# Sumar matrices
print("\nSuma A + B (Datosmatriz2.txt):")
print(m.suma_matrices("Datosmatriz2.txt"))

# Restar matrices
print("\nResta A - B:")
print(m.resta_matrices("Datosmatriz2.txt"))

# Multiplicar por escalar
print("\nA * 2:")
print(m.multi_escalar(2))

# Multiplicar matrices (si dimensiones compatibles)
print("\nProducto A * B (si columnas A == filas B):")
print(m.multi_matrices("Datosmatriz2.txt"))