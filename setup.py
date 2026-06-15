from pybind11.setup_helpers import Pybind11Extension
from setuptools import setup


ext_modules = [
    Pybind11Extension(
        "matrix1_cpp",
        ["matrixpy.cpp"],
        cxx_std=20,
        define_macros=[
            ("MATRIX1_DEBUG", "0"),
        ],
    ),
]


setup(
    name="matrix1_cpp",
    version="0.1.0",
    ext_modules=ext_modules,
)
