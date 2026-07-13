#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

int main()
{
    // Crear el cono
    vtkNew<vtkConeSource> cono;
    cono->SetResolution(30);

    // Preparar el cono para mostrarlo
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cono->GetOutputPort());

    // Crear el objeto visible
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Crear el escenario
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);

    // Crear la ventana
    vtkNew<vtkRenderWindow> ventana;
    ventana->AddRenderer(renderer);
    ventana->SetWindowName("Cono en VTK");

    // Permitir interactuar con la ventana
    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(ventana);

    ventana->Render();
    interactor->Start();

    return 0;
}