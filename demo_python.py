"""
Demostracion de los tres requisitos en Python sobre el modulo matrix1.
 
El modulo ahora expone DOS tipos (int y float):
    matrix1.Matrix1Int    -> Matrix1<int>
    matrix1.Matrix1Float  -> Matrix1<float>
 
Se prueban los tres requisitos con AMBOS tipos, para demostrar que la
plantilla funciona para int y para float.
 
Uso:
    python3 setup.py build_ext --inplace   # compila el modulo matrix1
    python3 demo_python.py
"""
 
import matrix1
 
 
def requisitos(Matrix, nombre, uno, dos):
    print("\n############  Tipo:", nombre, " ############")
 
    # ------------------------------------------------------------------
    # 1) Hacer posible m[3][2] = v
    #    m[3] devuelve una vista (memoryview) ESCRIBIBLE de la fila 3,
    #    apuntando a la memoria real de la matriz; [2] = v escribe ahi.
    # ------------------------------------------------------------------
    m = Matrix(4, 3)                 # 4 filas x 3 columnas, llena de 0
    m[3][2] = dos                    # asignacion por doble indice
    assert m[3, 2] == dos            # se confirma leyendo con m[i, j]
    print("1) m[3][2] =", dos, " ->  m[3,2] =", m[3, 2])
 
    # ------------------------------------------------------------------
    # 2) print(m)  (usa __str__/__repr__, que reusan operator<<)
    # ------------------------------------------------------------------
    print("2) print(m):")
    print(m)
 
    # ------------------------------------------------------------------
    # 3) m1 = m2 * m3  (multiplicacion de matrices)
    #    m2 (2x3) * m3 (3x2) -> 2x2; cada elemento = 3 * uno * dos
    # ------------------------------------------------------------------
    m2 = Matrix(2, 3, uno)           # 2x3 llena de 'uno'
    m3 = Matrix(3, 2, dos)           # 3x2 llena de 'dos'
    m1 = m2 * m3                     # 2x2
    print("3) m1 = m2 * m3:")
    print(m1)
    assert m1[0, 0] == 3 * uno * dos
 
    # ------------------------------------------------------------------
    # 4) Manejo de errores: dimensiones distintas -> excepcion
    # ------------------------------------------------------------------
    try:
        Matrix(2, 2, uno) + Matrix(3, 3, uno)
        assert False, "deberia haber lanzado excepcion"
    except (ValueError, RuntimeError) as e:
        print("4) suma con dimensiones distintas -> excepcion:", e)
 
 
def main():
    requisitos(matrix1.Matrix1Int,   "Matrix1Int",   1,   8)
    requisitos(matrix1.Matrix1Float, "Matrix1Float", 1.0, 8.0)
 
 
if __name__ == "__main__":
    main()
    print("\nOK: los tres requisitos funcionan para int y float.")
 