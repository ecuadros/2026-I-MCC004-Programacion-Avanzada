import matrix_lib

# 1. Crear matrices e inicializar la memoria (usando tu método Create)
m1 = matrix_lib.MatrixInt(2, 2)
m1.create()

m2 = matrix_lib.MatrixInt(2, 2)
m2.create()

# 2. Sintaxis encadenada m[i][j] (usando tu operator[])
m1[0][0] = 5
m1[0][1] = 2
m1[1][0] = 3
m1[1][1] = 1



# 3. Sintaxis de tupla m[i, j] (usando tu operator())
m2[0][0] = 2
m2[0][1] = 3
m2[1][0] = 4
m2[1][1] = 5

# 4. Operaciones matemáticas (usando tus operator+, -, *)
m3 = m1 + m2
m4 = m1 * m2

print("Matriz M1:")
print(m1)
print(m2)

print("Matriz M3 : (M1 + M2):")
print(m3)

print("Matriz M4 : (M1 * M2):")
print(m4)

# Liberar memoria manualmente si lo deseas, o dejar que el destructor lo haga
m1.destroy()
m2.destroy()
m3.destroy()
m4.destroy()

