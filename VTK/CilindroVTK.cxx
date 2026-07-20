#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

int main(int, char*[]) {
    // 1. Crear una paleta de colores
    vtkNew<vtkNamedColors> colors;

    // 2. FUENTE (Source): Crear el objeto geométrico (Cilindro)
    // Esto genera los datos matemáticos puros (vértices y polígonos)
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8); // Cilindro octagonal

    // 3. MAPEADOR (Mapper): Convierte los datos matemáticos en datos gráficos
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // 4. ACTOR: Representa el objeto en la escena (geometría + propiedades visuales)
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    // 5. RENDERIZADOR (Renderer): Crea la escena y añade los actores
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(cylinderActor);
    renderer->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    // 6. VENTANA DE RENDERIZADO (Render Window): La ventana del SO donde se dibuja todo
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetSize(600, 600);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Primer Ejemplo VTK - Cilindro");

    // 7. INTERACTOR: Permite usar el ratón para rotar/hacer zoom en la escena
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // 8. Iniciar el renderizado y el bucle de eventos del ratón
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
