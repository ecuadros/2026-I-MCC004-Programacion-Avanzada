"""
Wrapper simple para usar matrices C++ desde Python
"""

import ctypes
import os

class SimpleMatrix:
    
    def __init__(self):
        lib_path = self._get_library()
        # Convertir a ruta absoluta si es necesario
        if not os.path.isabs(lib_path):
            lib_path = os.path.join(os.path.dirname(__file__), lib_path)
        self.lib = ctypes.CDLL(lib_path)
        
        # Configurar tipos para funciones básicas
        self.lib.create_bridge.restype = ctypes.c_void_p
        self.lib.destroy_bridge.argtypes = [ctypes.c_void_p]
        self.lib.bridge_load.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.bridge_load.restype = ctypes.c_bool
        self.lib.bridge_toString.argtypes = [ctypes.c_void_p]
        self.lib.bridge_toString.restype = ctypes.c_char_p
        
        # Configurar tipos para operaciones matriciales
        self.lib.suma_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.suma_matrices.restype = ctypes.c_char_p
        self.lib.resta_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.resta_matrices.restype = ctypes.c_char_p
        self.lib.multi_escalar.argtypes = [ctypes.c_char_p, ctypes.c_double]
        self.lib.multi_escalar.restype = ctypes.c_char_p
        self.lib.multi_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.multi_matrices.restype = ctypes.c_char_p
        
        # Crear matriz C++
        self.obj = self.lib.create_bridge()
        self.current_file = None  # Para recordar el último archivo cargado

        # Configurar tipos para multiplicación paralela (solo si está en la librería)
        try:
            self.lib.multi_matrices_parallel.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint]
            self.lib.multi_matrices_parallel.restype = ctypes.c_char_p
        except AttributeError:
            pass  # La función no está disponible, pero no falla

    def _get_library(self):
        """Obtener o compilar librería"""
        lib_name = "libmatrix_bridge.so"
        
        if os.name == 'nt':  # Windows
            lib_name = "matrix_bridge.dll"
        elif os.name == 'posix':  # Linux/Mac
            lib_name = "libmatrix_bridge.so"
        
        if not os.path.exists(lib_name):
            self._compile()
        
        return lib_name
    
    def _compile(self):
        """Compilar librería C++"""
        print("Compilando...")
        
        if os.name == 'nt':  # Windows
            os.system("g++ -std=c++17 -shared matrix_bridge.cpp -o matrix_bridge.dll")
        else:  # Linux/Mac
            os.system("g++ -std=c++17 -fPIC -shared matrix_bridge.cpp -o libmatrix_bridge.so")
        
        print("Listo!")
    
    def load(self, filename):
        """Cargar matriz desde archivo y recordar el nombre"""
        if self.lib.bridge_load(self.obj, filename.encode()):
            self.current_file = filename
            return True
        return False
    
    def __str__(self):
        """Mostrar matriz"""
        result = self.lib.bridge_toString(self.obj)
        return result.decode()
    
    def __del__(self):
        """Liberar memoria"""
        if hasattr(self, 'lib') and hasattr(self, 'obj'):
            self.lib.destroy_bridge(self.obj)
    
    def suma_matrices(self, other_filename):
        if not self.current_file:
            raise Exception("Primero carga una matriz con load()")
        result = self.lib.suma_matrices(self.current_file.encode(), other_filename.encode())
        return result.decode()
    
    def resta_matrices(self, other_filename):
        if not self.current_file:
            raise Exception("Primero carga una matriz con load()")
        result = self.lib.resta_matrices(self.current_file.encode(), other_filename.encode())
        return result.decode()
    
    def multi_escalar(self, scalar):
        if not self.current_file:
            raise Exception("Primero carga una matriz con load()")
        result = self.lib.multi_escalar(self.current_file.encode(), float(scalar))
        return result.decode()
    
    def multi_matrices(self, other_filename):
        if not self.current_file:
            raise Exception("Primero carga una matriz con load()")
        result = self.lib.multi_matrices(self.current_file.encode(), other_filename.encode())
        return result.decode()
    
    


# Función auxiliar simple
def cargar_matriz(archivo):
    """Cargar matriz desde archivo y retornar su representación"""
    m = SimpleMatrix()
    if m.load(archivo):
        return str(m)
    return "Error al cargar"

def multiply_parallel(self, other_filename, num_threads=4):
    if not self.current_file:
        raise Exception("Primero carga una matriz con load()")
    # Asegurar que la función exista en la librería
    if not hasattr(self.lib, 'multi_matrices_parallel'):
        raise Exception("La librería no tiene multi_matrices_parallel. Compila con -pthread y la implementación en C++.")
    result = self.lib.multi_matrices_parallel(self.current_file.encode(), other_filename.encode(), num_threads)
    return result.decode()