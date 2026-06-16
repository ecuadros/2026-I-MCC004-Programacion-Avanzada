CXX = g++
# se agrega -fno-elide-constructors para evitar la optimización de la elisión de constructores
# y así ver los mensajes del move constructor en la salida
CXXFLAGS = -std=c++14 -fno-elide-constructors -Wall -g -pthread # Añadido -pthread
LDFLAGS = -pthread # Añadido -pthread

PYBIND_INC = $(shell python3 -m pybind11 --includes)
PY_SUFFIX  = $(shell python3-config --extension-suffix)
MODULE_FLG = -O3 -Wall -shared -std=c++14 -fPIC
MODULE_SRC = matrix_module.cpp
MODULE_OUT = matrix_module$(PY_SUFFIX)

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

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

module:
	$(CXX) $(MODULE_FLG) $(PYBIND_INC) $(MODULE_SRC) -o $(MODULE_OUT)

.PHONY: all clean