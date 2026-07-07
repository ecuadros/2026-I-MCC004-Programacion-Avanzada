import matrix1

if __name__ == "__main__":
    print("=" * 50)
    print("PRUEBAS DE MATRIX1 - USANDO MÉTODOS C++")
    print("=" * 50)
    
    # Test 1: Identity (estático)
    I = matrix1.Matrix1.identity(3)
    print("Matrix1.identity(3):")
    print(I)
    print()
    
    # Test 2: Transpose
    a = matrix1.Matrix1(2, 3, 0.0)
    a[0, 0] = 1.0
    a[0, 1] = 2.0
    a[0, 2] = 3.0
    a[1, 0] = 4.0
    a[1, 1] = 5.0
    a[1, 2] = 6.0
    
    print("A (2x3):")
    print(a)
    print()
    
    print("A.transpose() (3x2):")
    print(a.transpose())
    print()
    
    # Test 3: Trace con matriz NO cuadrada (debe lanzar excepción)
    try:
        print(f"A.trace() = {a.trace()}")
        print("ERROR: Debería haber lanzado excepción")
    except ValueError as e:
        print(f"Excepción capturada - {e}")
    
    # Test 4: Trace con matriz cuadrada
    I3 = matrix1.Matrix1.identity(3)
    print(f"identity(3).trace() = {I3.trace()}")
    
    print("\n" + "=" * 50)
    print("PRUEBAS COMPLETADAS")