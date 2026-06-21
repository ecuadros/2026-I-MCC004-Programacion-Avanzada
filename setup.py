from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
 
ext_modules = [
    Pybind11Extension(
        "matrix1",
        ["py_matrix.cpp"],
        cxx_std=17,          # structured bindings: auto [i, j]
        extra_compile_args=["-pthread"],
        extra_link_args=["-pthread"],
    ),
]
 
setup(
    name="matrix1",
    version="0.1",
    description="Matrix1 expuesta a Python con pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)