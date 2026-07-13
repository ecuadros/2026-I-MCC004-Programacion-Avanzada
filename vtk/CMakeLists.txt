cmake_minimum_required(VERSION 3.16)

project(ConoVTK)

find_package(VTK REQUIRED COMPONENTS
    CommonCore
    FiltersSources
    RenderingCore
    RenderingOpenGL2
    InteractionStyle
)

add_executable(cono cono.cpp)

target_link_libraries(cono PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
    TARGETS cono
    MODULES ${VTK_LIBRARIES}
)