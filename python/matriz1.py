"""
@file matriz1.py
@brief Demo de la clase Matrix1 desde Python usando el módulo MatrizToString.
"""
import MatrizToString as mts

def load(fname):
    with open(fname) as f:
        lines = f.readlines()
    rows, cols = map(int, lines[0].split())
    m = mts.MatrixD(rows, cols)   # <--- Usar MatrixD para double
    for i in range(rows):
        vals = lines[i+1].split()
        for j in range(cols):
            m.set(i, j, float(vals[j]))
    return m

m1 = load("Datamatriz1.txt")
m2 = load("Datosmatriz2.txt")
m3 = load("Datosmatriz3.txt")

print("m1:\n", m1)
print("m2:\n", m2)
print("m3:\n", m3)

print("m1+m2:\n", m1 + m2)
print("m1-m2:\n", m1 - m2)
print("m1*2.5:\n", m1 * 2.5)
print("m1*m2:\n", m1 * m2)
print("m1/2.0:\n", m1 / 2.0)

print("det(m1) =", m1.det())
print("det(m3) =", m3.det())

print("\nm1(1,1) antes:", m1(1,1))
m1.set(1, 1, 99.0)
print("\nm1(1,1) después:", m1(1,1))
print("\nm1 modificada:\n", m1)

print("m = 5*m2 + m1*m3:\n", 5*m2 + m1*m3)