"""Version en Python de Matrix1.

Este archivo reproduce la idea de Matrix1 en C++ utilizando sintaxis Python:

    m[3][2] = 8
    print(m)
    m1 = m2 * m3
    m1 = 5 * m2 + m3 * m4
"""

from __future__ import annotations
from concurrent.futures import ThreadPoolExecutor
from numbers import Number # Para verificar si un valor es un número (int, float, etc.)

## @class Matrix
## @brief Clase para representar matrices con indexacion, operaciones y demos.
##
## La clase permite crear matrices, acceder con doble indice, imprimirlas,
## multiplicarlas por escalares, multiplicarlas por otras matrices, sumar,
## restar, leer datos con expresiones regulares y ejecutar un demo con threads.
class Matrix:
    ## @brief Constructor para crear una matriz inicializada con un valor.
    ## @param rows Numero de filas.
    ## @param cols Numero de columnas.
    ## @param fill Valor inicial de cada celda.
    def __init__(self, rows, cols, fill=0):
        if rows <= 0 or cols <= 0:
            raise ValueError("El número de filas y columnas debe ser un número entero positivo")
        self.rows = rows
        self.cols = cols
        self.data = [[fill for _ in range(cols)] for _ in range(rows)]
        

    ## @brief Crea una matriz desde una lista de filas.
    ## @param rows Lista de listas con los datos de la matriz.
    ## @return Nueva matriz con los datos recibidos.
    @classmethod
    def from_rows(cls, rows):
        if not rows:
            raise ValueError("La entrada no puede estar vacía")

        data = [list(row) for row in rows]
        cols = len(data[0])
        if cols == 0:
            raise ValueError("Las filas no pueden estar vacías")

        for row in data:
            if len(row) != cols:
                raise ValueError("Todas las filas deben tener el mismo número de columnas")

        matrix = cls(len(data), cols) # Crea una matriz con las dimensiones correctas
        matrix.data = data 
        return matrix


    ## @brief Accede a una fila de la matriz.
    ##
    ## Esto permite m[3][2], porque m[3] devuelve la fila 3 y luego [2]
    ## accede a la columna 2.
    def __getitem__(self, index):
        return self.data[index]

    ## @brief Asigna una fila completa.
    ##
    ## Para una celda se usa m[fila][columna] = valor.
    def __setitem__(self, index, value):
        self.data[index] = value

    ## @brief Representacion usada por print(m).
    def __str__(self):
        lines = [f"Dimensiones Matrix: {self.rows} x {self.cols}"]
        for row in self.data:
            lines.append(" ".join(str(value) for value in row))
        return "\n".join(lines)

    ## @brief Simplifica la creacion de una matriz resultado.
    ##
    ## cell_func recibe i, j y devuelve el valor que debe guardarse en result[i][j].
    def _build(self, rows, cols, cell_func):
        result = Matrix(rows, cols)
        for i in range(rows):
            for j in range(cols):
                result[i][j] = cell_func(i, j)
        return result

    ## @brief Simplifica operaciones celda por celda, suma y resta.
    ##
    ## operation recibe dos valores: self[i][j] y other[i][j].
    def _elementwise(self, other, operation, operation_name):
        self._require_same_dimensions(other, operation_name)
        return self._build(
            self.rows,
            self.cols,
            lambda i, j: operation(self.data[i][j], other.data[i][j]),
        )

    ## @brief Verifica que dos matrices tengan las mismas dimensiones.
    def _require_same_dimensions(self, other, operation_name):
        if not isinstance(other, Matrix):
            raise TypeError("Solo puede funcionar con otra matriz")
        if self.rows != other.rows or self.cols != other.cols:
            raise ValueError(f"Las matrices deben tener las mismas dimensiones para {operation_name}.")

    ## @brief Suma dos matrices del mismo tamano.
    def __add__(self, other):
        return self._elementwise(other, lambda a, b: a + b, "suma")

    ## @brief Resta dos matrices del mismo tamano.
    def __sub__(self, other):
        return self._elementwise(other, lambda a, b: a - b, "resta")

    ## @brief Multiplica por una matriz o por un escalar
    def __mul__(self, other):
        ## Multiplicacion por otra matriz.
        if isinstance(other, Matrix):
            if self.cols != other.rows:
                raise ValueError(
                    "El número de columnas de la primera matriz debe ser igual al número de filas de la segunda matriz."
                )

            return self._build(
                self.rows,
                other.cols,
                lambda i, j: sum(self.data[i][k] * other.data[k][j] for k in range(self.cols)),
            )

        ## Multiplicacion escalar
        if isinstance(other, Number):
            return self._build(
                self.rows,
                self.cols,
                lambda i, j: self.data[i][j] * other,
            )

        return NotImplemented

    ## @brief Permite multiplicacion escalar desde la izquierda: 5 * matrix
    def __rmul__(self, value):
        if isinstance(value, Number):
            return self * value
        return NotImplemented

    ## @brief Extra #1: Devuelve la matriz transpuesta
    def transpose(self):
        return self._build(
            self.cols,
            self.rows,
            lambda i, j: self.data[j][i],
        )

    ## @brief Extra #2: Aplica una funcion a cada elemento
    def apply(self, func):
        return self._build(
            self.rows,
            self.cols,
            lambda i, j: func(self.data[i][j]),
        )

    ## @brief Extra #3: Calcula la suma diagonal (traza) de una matriz cuadrada
    def trace(self):
        if self.rows != self.cols:
            raise ValueError("El trazado requiere una matriz cuadrada")
        return sum(self.data[i][i] for i in range(self.rows))

    ## @brief Demo con Threads para multiplicar matrices.
    def multiply_threaded(self, other):
        if not isinstance(other, Matrix):
            raise TypeError("Solo se puede multiplicar por otra matriz")
        
        if self.cols != other.rows:
            raise ValueError(
                "El número de columnas de la primera matriz debe ser igual al número de filas de la segunda matriz"
            )

        result = Matrix(self.rows, other.cols) # Crea una matriz resultado con las dimensiones correctas

        def compute_cell(position): # Calcula el valor de una celda específica (i, j) en la matriz resultado
            i, j = position
            value = 0

            for k in range(self.cols): # Suma el producto de los elementos correspondientes de la fila i de self y la columna j de other
                value += self.data[i][k] * other.data[k][j]

            return i, j, value

        positions = [] # Genera una lista de todas las posiciones (i, j) que deben ser calculadas en la matriz resultado
        for i in range(result.rows):
            for j in range(result.cols):
                positions.append((i, j))

        with ThreadPoolExecutor() as executor: # Crea un grupo de threads para ejecutar compute_cell en paralelo para cada posición (i, j) en la matriz resultado
            for i, j, value in executor.map(compute_cell, positions):
                result[i][j] = value

        return result
