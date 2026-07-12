CXX = g++
CXXFLAGS = -std=c++2b -Wall -g -pthread
LDFLAGS = -pthread

TARGET = main
SRCS = main.cpp util.cpp \
       complex.cpp \
       shapes/shape.cpp \
       shapes/rectangle.cpp \
       shapes/circle.cpp \
       shapes/triangle.cpp \
       shapes/square.cpp \
       polimorfismo.cpp \
       BitSigno.cpp \
       Pointers.cpp \
       array1.cpp

OBJS = $(SRCS:.cpp=.o)

# ======================================================
# Objetivo para compilar el módulo pybind11 (MatrizToString.so)
# ======================================================
PYBIND_SRC = C++ToPython.cpp
PYBIND_TARGET = MatrizToString.so
PYBIND_DEPS = matrix1.h types.h

# Flags específicas para pybind11
PYBIND_CXXFLAGS = -std=c++2b -O3 -Wall -shared -fPIC

# Intentar obtener rutas con python3-config, si falla usar rutas comunes
PYTHON_CONFIG = python3-config
PYBIND_INCLUDES = $(shell $(PYTHON_CONFIG) --includes 2>/dev/null || echo "-I/usr/include/python3.12")
PYBIND_INCLUDES += -I/usr/lib/python3/dist-packages/pybind11/include

# Si pybind11 no está en /usr/lib, podrías ajustar la ruta aquí
# También se puede usar pkg-config: pkg-config --cflags pybind11

PYBIND_LDFLAGS = $(shell $(PYTHON_CONFIG) --ldflags 2>/dev/null || echo "-L/usr/lib/python3.12/config-3.12-x86_64-linux-gnu -lpython3.12")

# ======================================================
# Objetivos principales
# ======================================================
all: $(TARGET) $(PYBIND_TARGET)

$(TARGET): $(OBJS)
	@echo "🔨 Compilando ejecutable $(TARGET)..."
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar el módulo pybind11
pybind: $(PYBIND_TARGET)

$(PYBIND_TARGET): $(PYBIND_SRC) $(PYBIND_DEPS)
	@echo "🐍 Compilando módulo pybind11 $(PYBIND_TARGET)..."
	$(CXX) $(PYBIND_CXXFLAGS) $(PYBIND_INCLUDES) $(PYBIND_LDFLAGS) $< -o $@

# ======================================================
# Limpieza
# ======================================================
clean:
	@echo "🧹 Limpiando archivos objeto, ejecutable y módulo..."
	rm -f $(OBJS) $(TARGET) $(PYBIND_TARGET)

.PHONY: all clean pybind