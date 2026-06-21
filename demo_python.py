"""
Demostracion de los tres requisitos en Python sobre el modulo matrix1.
 
No requiere ningun cambio en el codigo C++: usa el binding que ya existe
en py_matrix.cpp (memoryview escribible por fila, __repr__/__str__ y
el operator* matriz-matriz).
 
Uso:
    python setup.py build_ext --inplace   # compila el modulo matrix1
    python demo_python.py
"""
 
import matrix1
 
 
def main():
    # ------------------------------------------------------------------
    # 1) Hacer posible m[3][2] = 8
    #    m[3] devuelve una vista (memoryview) ESCRIBIBLE de la fila 3,
    #    apuntando a la memoria real de la matriz; [2] = 8 escribe ahi.
    # ------------------------------------------------------------------
    m = matrix1.Matrix1(4, 3, 0.0)   # 4 filas x 3 columnas, llena de 0
    m[3][2] = 8                      # asignacion por doble indice
    assert m[3, 2] == 8              # se confirma leyendo con m[i, j]
    print("1) m[3][2] = 8  ->  m[3,2] =", m[3, 2])
 
    # ------------------------------------------------------------------
    # 2) print(m)
    #    Funciona gracias a __str__/__repr__, que reusan operator<<.
    # ------------------------------------------------------------------
    print("\n2) print(m):")
    print(m)
 
    # ------------------------------------------------------------------
    # 3) m1 = m2 * m3  (multiplicacion de matrices)
    #    Mapeado con .def(py::self * py::self) sobre tu operator*.
    #    m2 (2x3) * m3 (3x2) -> resultado 2x2; cada elemento = 1*2 * 3 = 6
    # ------------------------------------------------------------------
    m2 = matrix1.Matrix1(2, 3, 1.0)  # 2x3 llena de 1
    m3 = matrix1.Matrix1(3, 2, 2.0)  # 3x2 llena de 2
    m1 = m2 * m3                     # 2x2
    print("\n3) m1 = m2 * m3:")
    print(m1)
    assert m1[0, 0] == 6.0
 
    # ------------------------------------------------------------------
    # 4) Manejo de errores: operar matrices de distinto tamano debe
    #    lanzar excepcion (pybind11 traduce el std::invalid_argument
    #    de C++ a un ValueError en Python).
    # ------------------------------------------------------------------
    try:
        matrix1.Matrix1(2, 2, 1.0) + matrix1.Matrix1(3, 3, 1.0)
        assert False, "deberia haber lanzado excepcion"
    except (ValueError, RuntimeError) as e:
        print("\n4) suma con dimensiones distintas -> excepcion:", e)
 
 
if __name__ == "__main__":
    main()
    print("\nOK: los tres requisitos funcionan.")