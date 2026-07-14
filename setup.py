from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "matrix_lib",           # Nombre que tendrá el módulo en Python import matrix_lib
        ["matrixWrapper.cpp"], # Archivo C++ que acabamos de crear
        cxx_std=14              # Estándar C++14 para soportar std::exchange
    ),
]

setup(
    name="matrix_lib",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)

