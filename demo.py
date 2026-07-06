
from matrix_wrapper import SimpleMatrix, cargar_matriz

def main():

    m = SimpleMatrix()

    print("\n1. Cargando Datamatriz1.txt como Matriz A...")
    if not m.load("Datamatriz1.txt"):
        print("Error")
        return
    print("\n--- Matriz A ---")
    print(m)

    print("\n2. Cargando Datosmatriz2.txt como Matriz B...")
    resultado_b = cargar_matriz("Datosmatriz2.txt")
    if "Error" in resultado_b:
        print(f"Error{resultado_b}")
        return
    print("\n--- Matriz B ---")
    print(resultado_b)

    # Suma A + B
    print("\n3. Suma de matrices (A + B):")
    try:
        suma = m.suma_matrices("Datosmatriz2.txt")
        print(suma)
    except Exception as e:
        print(f"   Error: {e}")

    # Resta A - B
    print("\n4. Resta de matrices (A - B):")
    try:
        resta = m.resta_matrices("Datosmatriz2.txt")
        print(resta)
    except Exception as e:
        print(f"   Error: {e}")

    # Multiplicación escalar (A * 2.5)
    escalar = 2.5
    print(f"\n5. Multiplicación escalar (A * {escalar}):")
    try:
        escalar_res = m.multi_escalar(escalar)
        print(escalar_res)
    except Exception as e:
        print(f"   Error: {e}")

    # Multiplicación de matrices (A * B) – producto matricial
    print("\n6. Multiplicación de matrices (A * B):")
    try:
        producto = m.multi_matrices("Datosmatriz2.txt")
        print(producto)
    except Exception as e:
        print(f"   Error: {e}")

if __name__ == "__main__":
    main()