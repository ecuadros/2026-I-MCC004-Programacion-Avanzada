from setuptools import setup, Extension 
import pybind11

modulo = Extension(
    "matrix_py",
    sources=["matrix_py.cpp"],
    include_dirs=[pybind11.get_include()],
    language="c++"
)

setup(
    name="matrix_py",
    version="1.0",
    ext_modules=[modulo]
)