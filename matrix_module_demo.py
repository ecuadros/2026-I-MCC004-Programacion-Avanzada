from matrix_module import Matrix1Double

m2 = Matrix1Double(2, 2)
m2[0][0] = 1.0
m2[0][1] = 2.0
m2[1][0] = 3.0
m2[-1][-1] = 4.0
print("Shape of m2:", m2.shape)
print("Contents of m2:", m2)

m3 = Matrix1Double(2, 2)
m3[0][0] = 5.0
m3[0][1] = 6.0
m3[1][0] = 7.0
m3[-1][-1] = 8.0
print("\nShape of m3:", m3.shape)
print("Contents of m3:", m3)

m1 = m2 * m3

print("\nShape of m1:", m1.shape)
print("Contents of m1:", m1)
print("Value at m1[0][1]:", m1[0][1])
print("Value at m1[-2][1]", m1[-2][-1])
