// Demo pequeño de VTK: renderiza un cono 3D y lo guarda como PNG.
//
// Instalar VTK (Ubuntu/Debian):
//   sudo apt-get install libvtk9-dev
//
// Compilar (VTK requiere CMake, ver CMakeLists_vtk.txt):
//   mkdir build_vtk && cd build_vtk
//   cmake -S .. -B . -f ../CMakeLists_vtk.txt   # o copiar CMakeLists_vtk.txt como CMakeLists.txt
//   make
//   ./vtk_demo
//
// En un contenedor sin display, correr con un display virtual:
//   sudo apt-get install xvfb
//   xvfb-run -a ./vtk_demo      # genera cono.png igual

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkNew.h>
#include <iostream>

int main()
{
    // 1. Fuente: un cono con 30 caras laterales
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(30);

    // 2. Mapper: convierte la geometria en primitivas de render
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cone->GetOutputPort());

    // 3. Actor: el objeto que se dibuja, con color y posicion
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.2, 0.6, 0.9);

    // 4. Renderer + ventana
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.15);

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetSize(640, 480);
    window->SetOffScreenRendering(1); // funciona sin display
    window->Render();

    // 5. Guardar la imagen a PNG
    vtkNew<vtkWindowToImageFilter> w2i;
    w2i->SetInput(window);
    w2i->Update();

    vtkNew<vtkPNGWriter> writer;
    writer->SetFileName("cono.png");
    writer->SetInputConnection(w2i->GetOutputPort());
    writer->Write();

    std::cout << "Imagen guardada en cono.png\n";
    return 0;
}
