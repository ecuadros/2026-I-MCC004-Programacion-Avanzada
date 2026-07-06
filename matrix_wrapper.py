import ctypes
import os

class SimpleMatrix:
    
    def __init__(self):
        lib_path = self._get_library()
        if not os.path.isabs(lib_path):
            lib_path = os.path.join(os.path.dirname(__file__), lib_path)
        self.lib = ctypes.CDLL(lib_path)
        
        self.lib.create_bridge.restype = ctypes.c_void_p
        self.lib.destroy_bridge.argtypes = [ctypes.c_void_p]
        self.lib.bridge_load.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.bridge_load.restype = ctypes.c_bool
        self.lib.bridge_toString.argtypes = [ctypes.c_void_p]
        self.lib.bridge_toString.restype = ctypes.c_char_p
        
        self.lib.suma_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.suma_matrices.restype = ctypes.c_char_p
        self.lib.resta_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.resta_matrices.restype = ctypes.c_char_p
        self.lib.multi_escalar.argtypes = [ctypes.c_char_p, ctypes.c_double]
        self.lib.multi_escalar.restype = ctypes.c_char_p
        self.lib.multi_matrices.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        self.lib.multi_matrices.restype = ctypes.c_char_p

        self.obj = self.lib.create_bridge()
        self.current_file = None 

        try:
            self.lib.multi_matrices_parallel.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint]
            self.lib.multi_matrices_parallel.restype = ctypes.c_char_p
        except AttributeError:
            pass 

    def _get_library(self):

        lib_name = "libmatrix_bridge.so"
        
        if os.name == 'nt': 
            lib_name = "matrix_bridge.dll"
        elif os.name == 'posix': 
            lib_name = "libmatrix_bridge.so"
        
        if not os.path.exists(lib_name):
            self._compile()
        
        return lib_name
    
    def _compile(self):
        if os.name == 'nt':
            os.system("g++ -std=c++17 -shared matrix_bridge.cpp -o matrix_bridge.dll")
        else: 
            os.system("g++ -std=c++17 -fPIC -shared matrix_bridge.cpp -o libmatrix_bridge.so")
    
    def load(self, filename):
    
        if self.lib.bridge_load(self.obj, filename.encode()):
            self.current_file = filename
            return True
        return False
    
    def __str__(self):
        result = self.lib.bridge_toString(self.obj)
        return result.decode()
    
    def __del__(self):
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

def cargar_matriz(archivo):
    m = SimpleMatrix()
    if m.load(archivo):
        return str(m)
    return "Error"
