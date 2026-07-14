#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkFollower.h>
#include <vtkLineSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>
#include "types.h"

#include <string>

struct DigitalNode {
    TI          value = 0;
    DigitalNode *left = nullptr;
    DigitalNode *right = nullptr;
};

void AddEdge(vtkRenderer *renderer, vtkNamedColors *colors,
             T5 x1, T5 y1, T5 z1,
             T5 x2, T5 y2, T5 z2) {
    vtkNew<vtkLineSource> line;
    line->SetPoint1(x1, y1, z1);
    line->SetPoint2(x2, y2, z2);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(line->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("LightSteelBlue").GetData());
    actor->GetProperty()->SetLineWidth(4.0);

    renderer->AddActor(actor);
}

void AddNode(vtkRenderer *renderer, vtkNamedColors *colors,
             T5 x, T5 y, T5 z, int level) {
    vtkNew<vtkSphereSource> sphere;
    sphere->SetCenter(x, y, z);
    sphere->SetRadius(0.32);
    sphere->SetThetaResolution(32);
    sphere->SetPhiResolution(32);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphere->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    if (level == 0)
        actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    else if (level == 1)
        actor->GetProperty()->SetColor(colors->GetColor3d("MediumSeaGreen").GetData());
    else
        actor->GetProperty()->SetColor(colors->GetColor3d("DeepSkyBlue").GetData());

    actor->GetProperty()->SetSpecular(0.35);
    actor->GetProperty()->SetSpecularPower(20.0);

    renderer->AddActor(actor);
}

void AddLabel(vtkRenderer *renderer, vtkNamedColors *colors,
              TI value, T5 x, T5 y, T5 z) {
    vtkNew<vtkVectorText> text;
    text->SetText(std::to_string(value).c_str());

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(text->GetOutputPort());

    vtkNew<vtkFollower> label;
    label->SetMapper(mapper);
    label->SetScale(0.22, 0.22, 0.22);
    label->SetPosition(x - 0.15, y - 0.08, z + 0.36);
    label->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
    label->SetCamera(renderer->GetActiveCamera());

    renderer->AddActor(label);
}

void DrawDigitalTree(vtkRenderer *renderer, vtkNamedColors *colors,
                     DigitalNode *node, T5 x, T5 y, T5 z,
                     T5 offset, int level) {
    if (node == nullptr)
        return;

    AddNode(renderer, colors, x, y, z, level);
    AddLabel(renderer, colors, node->value, x, y, z);

    const T5 childY = y - 1.35;
    const T5 childOffset = offset * 0.55;

    if (node->left != nullptr) {
        const T5 childX = x - offset;
        const T5 childZ = z - 0.45;
        AddEdge(renderer, colors, x, y, z, childX, childY, childZ);
        DrawDigitalTree(renderer, colors, node->left, childX, childY, childZ, childOffset, level + 1);
    }

    if (node->right != nullptr) {
        const T5 childX = x + offset;
        const T5 childZ = z + 0.45;
        AddEdge(renderer, colors, x, y, z, childX, childY, childZ);
        DrawDigitalTree(renderer, colors, node->right, childX, childY, childZ, childOffset, level + 1);
    }
}

int main() {
    vtkNew<vtkNamedColors> colors;

    DigitalNode root{TI{10}};
    DigitalNode node5{TI{5}};
    DigitalNode node15{TI{15}};
    DigitalNode node3{TI{3}};
    DigitalNode node7{TI{7}};
    DigitalNode node20{TI{20}};

    root.left = &node5;
    root.right = &node15;
    node5.left = &node3;
    node5.right = &node7;
    node15.right = &node20;

    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    vtkCamera *camera = renderer->GetActiveCamera();
    camera->SetPosition(0.0, 0.0, 9.0);
    camera->SetFocalPoint(0.0, 0.0, 0.0);
    camera->SetViewUp(0.0, 1.0, 0.0);

    DrawDigitalTree(renderer, colors, &root, 0.0, 2.0, 0.0, 1.8, 0);
    renderer->ResetCamera();

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Demo VTK - Digital Tree 3D");
    renderWindow->SetSize(900, 700);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();

    return 0;
}
