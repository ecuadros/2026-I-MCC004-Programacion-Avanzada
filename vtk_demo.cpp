// Demo pequeño de VTK: renderiza un cono 3D.
//
// Si hay display disponible (variable DISPLAY), abre una ventana
// INTERACTIVA: arrastrar = rotar, rueda = zoom, 'q' = salir.
// Si no hay display (contenedor sin pantalla), guarda cono.png.
//
// Instalar VTK (Ubuntu/Debian):
//   sudo apt-get install libvtk9-dev cmake
//
// Compilar (VTK requiere CMake, ver CMakeLists_vtk.txt):
//   cp CMakeLists_vtk.txt CMakeLists.txt
//   mkdir -p build_vtk && cd build_vtk
//   cmake .. && make
//
// Para generar el PNG (aun sin pantalla) se usa un display virtual
// y el flag --png que fuerza el modo captura:
//   sudo apt-get install xvfb
//   xvfb-run -a ./vtk_demo --png
 
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkNew.h>
#include <cstdlib>
#include <string>
#include "types.h"
#include <iostream>
 
int main(int argc, char *argv[])
{   // --png fuerza guardar imagen en vez de abrir la ventana interactiva
    BoolType modo_png = (argc > 1 && std::string(argv[1]) == "--png");
 
    // 1. Fuente: un cono con 30 caras laterales
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(30);
 
    // 2. Mapper: convierte la geometria en poligonos dibujables
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cone->GetOutputPort());
 
    // 3. Actor: el objeto en escena, con su color
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.2, 0.4, 0.8);   // azul
 
    // 4. Renderer + ventana
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.15);
 
    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetSize(800, 600);
    window->SetWindowName("Demo VTK - Cono");
 
    // 5. Modo: interactivo si hay DISPLAY y no se pidio --png
    const char *display = std::getenv("DISPLAY");
    if (!modo_png && display != nullptr && display[0] != '\0')
    {   // Ventana interactiva: el interactor procesa mouse y teclado.
        // TrackballCamera: arrastrar rota la camara alrededor del objeto.
        vtkNew<vtkRenderWindowInteractor> interactor;
        interactor->SetRenderWindow(window);
        vtkNew<vtkInteractorStyleTrackballCamera> style;
        interactor->SetInteractorStyle(style);
 
        window->Render();
        std::cout << "Ventana interactiva: arrastre para rotar, rueda para zoom, 'q' para salir\n";
        interactor->Start();      // loop de eventos: bloquea hasta que cierres
    }
    else
    {   // Sin pantalla: renderizar offscreen y guardar PNG
        window->SetOffScreenRendering(1);
        window->Render();
 
        vtkNew<vtkWindowToImageFilter> capture;
        capture->SetInput(window);
        capture->Update();
 
        vtkNew<vtkPNGWriter> writer;
        writer->SetFileName("cono.png");
        writer->SetInputConnection(capture->GetOutputPort());
        writer->Write();
        std::cout << "Imagen guardada en cono.png\n";
    }
    return 0;
}
 