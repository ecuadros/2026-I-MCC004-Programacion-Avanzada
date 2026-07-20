/**
 * @file Proyecto.cpp
 * @brief Implementación de la simulación de radioterapia.
 * @details Define la lógica de cálculo de dosis, generación de la malla,
 *          construcción de la escena VTK y actualización interactiva.
 * @author (Tu nombre o el del equipo)
 * @date 2026
 */



#include "proyecto.h"

#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkCubeAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkImageData.h>
#include <vtkContourFilter.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>
#include <vtkMath.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkImageMapper3D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <cmath>
#include <sstream>

// ============================================================
// Parámetros globales
// ============================================================
const double PHANTOM_SIZE = 50.0;          // cm
const double PHANTOM_HALF = PHANTOM_SIZE / 2.0;
const int GRID_DIMS = 80;                  // resolución de la malla de dosis
const double MAX_DOSE = 100.0;             // dosis máxima normalizada

// Estado de la simulación
struct SimulationState {
    // Partícula y energía
    std::string particle;   // "foton", "neutron", "electron", "proton"
    double energyMV;        // 6, 10, 15, 20

    // Actores principales
    vtkActor* phantomActor;
    vtkActor* beamActor;
    vtkActor* dosePointsActor;
    vtkActor* isoActors[5];   // 90%, 70%, 50%, 30%, 10%
    vtkImageActor* sliceActor;
    vtkActor* pddCurveActor;
    vtkTextActor* infoActor;

    vtkRenderer* renderer;
    vtkRenderWindow* window;
    vtkRenderWindowInteractor* interactor;

    // Datos de dosis actuales (para regenerar)
    vtkSmartPointer<vtkImageData> doseImage;
    vtkSmartPointer<vtkLookupTable> doseLUT;
};

// ============================================================
// Funciones de dosis para cada partícula y energía
// ============================================================

// Dosis en función de la profundidad z (cm) y distancia radial r (cm) desde el eje central.
// Retorna dosis normalizada entre 0 y MAX_DOSE.
double CalculateDose(double z, double r, const std::string& particle, double energyMV) {
    // Profundidad desde la superficie (z=0 en la cara superior del fantoma)
    // El fantoma está centrado en el origen, por lo que la superficie superior está en z = +PHANTOM_HALF.
    // El haz incide desde z = +PHANTOM_HALF hacia z = -PHANTOM_HALF.
    double depth = PHANTOM_HALF - z;  // profundidad en cm (0 en superficie)

    // Si estamos fuera del fantoma, dosis 0
    if (depth < 0 || depth > PHANTOM_SIZE) return 0.0;
    if (fabs(r) > PHANTOM_HALF) return 0.0;

    // Atenuación lateral (gaussiana)
    double sigma = 2.5 + 0.2 * energyMV;  // dispersión lateral aumenta con energía
    double lateralFactor = exp(- (r*r) / (2.0 * sigma*sigma));

    // Perfil de profundidad (PDD) según partícula
    double pdd = 0.0;
    double Rmax = 0.0; // rango máximo (para protones)

    if (particle == "foton") {
        // Fotones: máximo de dosis a cierta profundidad (build-up), luego exponencial
        double dmax = 1.5 + 0.3 * energyMV; // profundidad del máximo (cm)
        double mu = 0.04 + 0.002 * energyMV; // coeficiente de atenuación (1/cm)
        // Factor de build-up simple
        double buildup = 1.0 + 0.2 * depth / dmax;
        pdd = buildup * exp(-mu * depth);
        // Normalizar para que el máximo sea 1
        double maxVal = (1.0 + 0.2) * exp(-mu * dmax);
        pdd /= maxVal;
    }
    else if (particle == "electron") {
        // Electrones: pico superficial, decaimiento exponencial
        double R50 = 2.0 + 1.5 * energyMV; // rango al 50%
        if (depth < R50) {
            pdd = exp(-depth * depth / (2.0 * (R50/2.5)*(R50/2.5)));
        } else {
            pdd = exp(- (depth - R50) / (R50/2.0) );
        }
        // Normalizar
        double maxVal = 1.0;
        pdd /= maxVal;
    }
    else if (particle == "proton") {
        // Protones: pico de Bragg
        double range = 1.5 + 2.2 * energyMV; // rango en cm (aproximado)
        // Distribución de Bragg: pico estrecho al final del rango
        double sigmaBragg = 1.2;
        pdd = exp(- (depth - range)*(depth - range) / (2.0 * sigmaBragg*sigmaBragg) );
        // También atenuación antes del pico
        if (depth < range - 2*sigmaBragg) {
            pdd *= 0.5; // dosis de entrada más baja
        }
        // Normalizar
        double maxVal = 1.0;
        pdd /= maxVal;
    }
    else if (particle == "neutron") {
        // Neutrones: penetran profundamente, dosis más uniforme
        double mu = 0.01 + 0.001 * energyMV;
        pdd = exp(-mu * depth);
        // Normalizar en superficie
        double maxVal = 1.0;
        pdd /= maxVal;
    }
    else {
        pdd = 0.0;
    }

    double dose = MAX_DOSE * pdd * lateralFactor;
    // Limitar a MAX_DOSE
    if (dose > MAX_DOSE) dose = MAX_DOSE;
    if (dose < 0.0) dose = 0.0;
    return dose;
}

// ============================================================
// Función para generar la malla de dosis
// ============================================================
vtkSmartPointer<vtkImageData> GenerateDoseImage(const std::string& particle, double energyMV) {
    int dims = GRID_DIMS;
    double spacing = PHANTOM_SIZE / (dims - 1);
    double origin = -PHANTOM_HALF;

    vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
    image->SetDimensions(dims, dims, dims);
    image->SetSpacing(spacing, spacing, spacing);
    image->SetOrigin(origin, origin, origin);

    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetNumberOfComponents(1);
    scalars->SetName("Dosis");

    for (int k = 0; k < dims; ++k) {
        for (int j = 0; j < dims; ++j) {
            for (int i = 0; i < dims; ++i) {
                double x = origin + i * spacing;
                double y = origin + j * spacing;
                double z = origin + k * spacing;
                double r = sqrt(x*x + y*y); // distancia radial al eje central
                double dose = CalculateDose(z, r, particle, energyMV);
                scalars->InsertNextValue(dose);
            }
        }
    }
    image->GetPointData()->SetScalars(scalars);
    return image;
}

// ============================================================
// Construcción de la escena
// ============================================================
void BuildScene(SimulationState& state) {
    vtkRenderer* renderer = state.renderer;

    // --- 1. Fantoma de agua ---
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    cube->SetXLength(PHANTOM_SIZE);
    cube->SetYLength(PHANTOM_SIZE);
    cube->SetZLength(PHANTOM_SIZE);
    cube->SetCenter(0.0, 0.0, 0.0);
    cube->Update();

    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputConnection(cube->GetOutputPort());

    vtkSmartPointer<vtkActor> phantomActor = vtkSmartPointer<vtkActor>::New();
    phantomActor->SetMapper(cubeMapper);
    phantomActor->GetProperty()->SetColor(0.0, 0.5, 0.8);
    phantomActor->GetProperty()->SetOpacity(0.08);
    phantomActor->GetProperty()->SetRepresentationToSurface();
    renderer->AddActor(phantomActor);
    state.phantomActor = phantomActor;

    // --- 2. Haz incidente (flecha o cono) ---
    // Un cono que apunta hacia abajo desde z=+PHANTOM_HALF hasta z=0
    vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
    cone->SetHeight(PHANTOM_HALF * 0.8);
    cone->SetRadius(3.0);
    cone->SetResolution(30);
    cone->SetDirection(0.0, 0.0, -1.0); // apunta hacia -Z
    cone->SetCenter(0.0, 0.0, PHANTOM_HALF * 0.5); // colocado en la parte superior
    cone->Update();

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(cone->GetOutputPort());

    vtkSmartPointer<vtkActor> beamActor = vtkSmartPointer<vtkActor>::New();
    beamActor->SetMapper(coneMapper);
    beamActor->GetProperty()->SetColor(1.0, 0.8, 0.0);
    beamActor->GetProperty()->SetOpacity(0.6);
    renderer->AddActor(beamActor);
    state.beamActor = beamActor;

    // --- 3. Malla de dosis y puntos ---
    // Generar imagen de dosis
    state.doseImage = GenerateDoseImage(state.particle, state.energyMV);

    // Mapa de colores
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange(0.0, 0.7); // Azul -> Rojo
    lut->SetRange(0.0, MAX_DOSE);
    lut->Build();
    state.doseLUT = lut;

    // Puntos muestreados (para visualización 3D)
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> doseArray = vtkSmartPointer<vtkDoubleArray>::New();
    doseArray->SetName("Dosis");
    vtkImageData* img = state.doseImage;
    int dims[3];
    img->GetDimensions(dims);
    double spacing[3];
    img->GetSpacing(spacing);
    double origin[3];
    img->GetOrigin(origin);

    // Muestrear cada 2 puntos para no saturar
    for (int i = 0; i < dims[0]; i += 2) {
        for (int j = 0; j < dims[1]; j += 2) {
            for (int k = 0; k < dims[2]; k += 2) {
                double x = origin[0] + i * spacing[0];
                double y = origin[1] + j * spacing[1];
                double z = origin[2] + k * spacing[2];
                double dose = img->GetPointData()->GetScalars()->GetTuple1(i + j*dims[0] + k*dims[0]*dims[1]);
                if (dose > 1.0) {
                    points->InsertNextPoint(x, y, z);
                    doseArray->InsertNextValue(dose);
                }
            }
        }
    }

    vtkSmartPointer<vtkPolyData> dosePolyData = vtkSmartPointer<vtkPolyData>::New();
    dosePolyData->SetPoints(points);
    dosePolyData->GetPointData()->SetScalars(doseArray);

    vtkSmartPointer<vtkDataSetMapper> pointMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    pointMapper->SetInputData(dosePolyData);
    pointMapper->SetLookupTable(lut);
    pointMapper->ScalarVisibilityOn();

    vtkSmartPointer<vtkActor> doseActor = vtkSmartPointer<vtkActor>::New();
    doseActor->SetMapper(pointMapper);
    doseActor->GetProperty()->SetPointSize(3);
    renderer->AddActor(doseActor);
    state.dosePointsActor = doseActor;

    // --- 4. Curvas de isodosis (superficies de nivel) ---
    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputData(state.doseImage);
    double isoLevels[5] = {90.0, 70.0, 50.0, 30.0, 10.0};
    for (int i = 0; i < 5; ++i) {
        contour->SetValue(i, isoLevels[i]);
    }
    contour->Update();

    vtkSmartPointer<vtkPolyDataMapper> isoMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    isoMapper->SetInputConnection(contour->GetOutputPort());
    isoMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
    isoActor->SetMapper(isoMapper);
    isoActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    isoActor->GetProperty()->SetOpacity(0.4);
    isoActor->GetProperty()->SetLineWidth(2);
    renderer->AddActor(isoActor);
    for (int i = 0; i < 5; ++i) state.isoActors[i] = isoActor; // compartimos el mismo actor, pero podríamos separarlos

    // --- 5. Corte axial (plano YZ en x=0) ---
    // Extraer el plano x=0 de la imagen
    vtkSmartPointer<vtkImageData> slice = vtkSmartPointer<vtkImageData>::New();
    slice->SetDimensions(1, dims[1], dims[2]);
    slice->SetSpacing(spacing[0], spacing[1], spacing[2]);
    slice->SetOrigin(0.0, origin[1], origin[2]);

    vtkSmartPointer<vtkDoubleArray> sliceScalars = vtkSmartPointer<vtkDoubleArray>::New();
    sliceScalars->SetNumberOfComponents(1);
    sliceScalars->SetName("Dosis");
    int xIdx = dims[0]/2; // índice para x=0
    for (int k = 0; k < dims[2]; ++k) {
        for (int j = 0; j < dims[1]; ++j) {
            int idx = xIdx + j*dims[0] + k*dims[0]*dims[1];
            double dose = img->GetPointData()->GetScalars()->GetTuple1(idx);
            sliceScalars->InsertNextValue(dose);
        }
    }
    slice->GetPointData()->SetScalars(sliceScalars);

    vtkSmartPointer<vtkImageMapToColors> mapToColors = vtkSmartPointer<vtkImageMapToColors>::New();
    mapToColors->SetInputData(slice);
    mapToColors->SetLookupTable(lut);
    mapToColors->Update();

    vtkSmartPointer<vtkImageActor> sliceActor = vtkSmartPointer<vtkImageActor>::New();
    sliceActor->SetInputData(mapToColors->GetOutput());
    sliceActor->SetDisplayExtent(0, 0, 0, dims[1]-1, 0, dims[2]-1);
    sliceActor->SetVisibility(true);
    renderer->AddActor(sliceActor);
    state.sliceActor = sliceActor;

    // --- 6. Curva de dosis en profundidad (PDD) a lo largo del eje central ---
    vtkSmartPointer<vtkPoints> pddPoints = vtkSmartPointer<vtkPoints>::New();
    int numPoints = 100;
    for (int i = 0; i < numPoints; ++i) {
        double z = -PHANTOM_HALF + i * (PHANTOM_SIZE / (numPoints-1));
        double dose = CalculateDose(z, 0.0, state.particle, state.energyMV);
        pddPoints->InsertNextPoint(0.0, 0.0, z);
        // Almacenaremos la dosis como un array escalar, pero para la línea usaremos un PolyLine
    }

    // Para dibujar una línea, necesitamos un polydata con una polyline
    vtkSmartPointer<vtkPolyData> pddPolyData = vtkSmartPointer<vtkPolyData>::New();
    pddPolyData->SetPoints(pddPoints);

    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType* ids = new vtkIdType[numPoints];
    for (int i = 0; i < numPoints; ++i) ids[i] = i;
    lines->InsertNextCell(numPoints, ids);
    delete[] ids;
    pddPolyData->SetLines(lines);

    // Además, queremos que el color de la línea represente la dosis a lo largo de la curva.
    // Asignamos escalares a los puntos.
    vtkSmartPointer<vtkDoubleArray> pddScalars = vtkSmartPointer<vtkDoubleArray>::New();
    pddScalars->SetNumberOfComponents(1);
    pddScalars->SetName("Dosis");
    for (int i = 0; i < numPoints; ++i) {
        double z = -PHANTOM_HALF + i * (PHANTOM_SIZE / (numPoints-1));
        double dose = CalculateDose(z, 0.0, state.particle, state.energyMV);
        pddScalars->InsertNextValue(dose);
    }
    pddPolyData->GetPointData()->SetScalars(pddScalars);

    vtkSmartPointer<vtkPolyDataMapper> pddMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pddMapper->SetInputData(pddPolyData);
    pddMapper->SetLookupTable(lut);
    pddMapper->ScalarVisibilityOn();

    vtkSmartPointer<vtkActor> pddActor = vtkSmartPointer<vtkActor>::New();
    pddActor->SetMapper(pddMapper);
    pddActor->GetProperty()->SetLineWidth(4);
    renderer->AddActor(pddActor);
    state.pddCurveActor = pddActor;

    // --- 7. Ejes con coordenadas ---
    vtkSmartPointer<vtkCubeAxesActor> axes = vtkSmartPointer<vtkCubeAxesActor>::New();
    axes->SetBounds(-PHANTOM_HALF, PHANTOM_HALF, -PHANTOM_HALF, PHANTOM_HALF, -PHANTOM_HALF, PHANTOM_HALF);
    axes->SetXTitle("X (cm)");
    axes->SetYTitle("Y (cm)");
    axes->SetZTitle("Z (cm)");
    axes->GetTitleTextProperty(0)->SetColor(1.0, 1.0, 1.0);
    axes->GetLabelTextProperty(0)->SetColor(0.8, 0.8, 0.8);
    axes->SetFlyModeToOuterEdges();
    renderer->AddActor(axes);

    // --- 8. Texto informativo ---
    vtkSmartPointer<vtkTextActor> info = vtkSmartPointer<vtkTextActor>::New();
    info->SetPosition(10, 10);
    info->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    info->GetTextProperty()->SetFontSize(16);
    std::ostringstream oss;
    oss << "Partícula: " << state.particle << "  Energía: " << state.energyMV << " MV\n";
    oss << "Teclas: [F] Fotones [N] Neutrones [E] Electrones [P] Protones\n";
    oss << "        [6] 6MV  [0] 10MV  [5] 15MV  [2] 20MV";
    info->SetInput(oss.str().c_str());
    renderer->AddActor(info);
    state.infoActor = info;

    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetPosition(-70, -40, 50);
    renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
}

// ============================================================
// Función para actualizar la escena al cambiar partícula/energía
// ============================================================
void UpdateScene(SimulationState& state) {
    vtkRenderer* renderer = state.renderer;

    // Eliminar actores dinámicos (los que vamos a regenerar)
    renderer->RemoveActor(state.dosePointsActor);
    renderer->RemoveActor(state.isoActors[0]);
    renderer->RemoveActor(state.sliceActor);
    renderer->RemoveActor(state.pddCurveActor);
    renderer->RemoveActor(state.infoActor);

    // Generar nueva dosis
    state.doseImage = GenerateDoseImage(state.particle, state.energyMV);

    // Recrear puntos de dosis
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> doseArray = vtkSmartPointer<vtkDoubleArray>::New();
    doseArray->SetName("Dosis");
    vtkImageData* img = state.doseImage;
    int dims[3];
    img->GetDimensions(dims);
    double spacing[3];
    img->GetSpacing(spacing);
    double origin[3];
    img->GetOrigin(origin);

    for (int i = 0; i < dims[0]; i += 2) {
        for (int j = 0; j < dims[1]; j += 2) {
            for (int k = 0; k < dims[2]; k += 2) {
                double x = origin[0] + i * spacing[0];
                double y = origin[1] + j * spacing[1];
                double z = origin[2] + k * spacing[2];
                double dose = img->GetPointData()->GetScalars()->GetTuple1(i + j*dims[0] + k*dims[0]*dims[1]);
                if (dose > 1.0) {
                    points->InsertNextPoint(x, y, z);
                    doseArray->InsertNextValue(dose);
                }
            }
        }
    }

    vtkSmartPointer<vtkPolyData> dosePolyData = vtkSmartPointer<vtkPolyData>::New();
    dosePolyData->SetPoints(points);
    dosePolyData->GetPointData()->SetScalars(doseArray);

    vtkSmartPointer<vtkDataSetMapper> pointMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    pointMapper->SetInputData(dosePolyData);
    pointMapper->SetLookupTable(state.doseLUT);
    pointMapper->ScalarVisibilityOn();

    vtkSmartPointer<vtkActor> doseActor = vtkSmartPointer<vtkActor>::New();
    doseActor->SetMapper(pointMapper);
    doseActor->GetProperty()->SetPointSize(3);
    renderer->AddActor(doseActor);
    state.dosePointsActor = doseActor;

    // Isodosis
    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputData(state.doseImage);
    double isoLevels[5] = {90.0, 70.0, 50.0, 30.0, 10.0};
    for (int i = 0; i < 5; ++i) contour->SetValue(i, isoLevels[i]);
    contour->Update();

    vtkSmartPointer<vtkPolyDataMapper> isoMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    isoMapper->SetInputConnection(contour->GetOutputPort());
    isoMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
    isoActor->SetMapper(isoMapper);
    isoActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    isoActor->GetProperty()->SetOpacity(0.4);
    isoActor->GetProperty()->SetLineWidth(2);
    renderer->AddActor(isoActor);
    for (int i = 0; i < 5; ++i) state.isoActors[i] = isoActor;

    // Slice axial (x=0)
    vtkSmartPointer<vtkImageData> slice = vtkSmartPointer<vtkImageData>::New();
    slice->SetDimensions(1, dims[1], dims[2]);
    slice->SetSpacing(spacing[0], spacing[1], spacing[2]);
    slice->SetOrigin(0.0, origin[1], origin[2]);

    vtkSmartPointer<vtkDoubleArray> sliceScalars = vtkSmartPointer<vtkDoubleArray>::New();
    sliceScalars->SetNumberOfComponents(1);
    sliceScalars->SetName("Dosis");
    int xIdx = dims[0]/2;
    for (int k = 0; k < dims[2]; ++k) {
        for (int j = 0; j < dims[1]; ++j) {
            int idx = xIdx + j*dims[0] + k*dims[0]*dims[1];
            double dose = img->GetPointData()->GetScalars()->GetTuple1(idx);
            sliceScalars->InsertNextValue(dose);
        }
    }
    slice->GetPointData()->SetScalars(sliceScalars);

    vtkSmartPointer<vtkImageMapToColors> mapToColors = vtkSmartPointer<vtkImageMapToColors>::New();
    mapToColors->SetInputData(slice);
    mapToColors->SetLookupTable(state.doseLUT);
    mapToColors->Update();

    vtkSmartPointer<vtkImageActor> sliceActor = vtkSmartPointer<vtkImageActor>::New();
    sliceActor->SetInputData(mapToColors->GetOutput());
    sliceActor->SetDisplayExtent(0, 0, 0, dims[1]-1, 0, dims[2]-1);
    sliceActor->SetVisibility(true);
    renderer->AddActor(sliceActor);
    state.sliceActor = sliceActor;

    // Curva PDD
    vtkSmartPointer<vtkPoints> pddPoints = vtkSmartPointer<vtkPoints>::New();
    int numPoints = 100;
    for (int i = 0; i < numPoints; ++i) {
        double z = -PHANTOM_HALF + i * (PHANTOM_SIZE / (numPoints-1));
        double dose = CalculateDose(z, 0.0, state.particle, state.energyMV);
        pddPoints->InsertNextPoint(0.0, 0.0, z);
    }

    vtkSmartPointer<vtkPolyData> pddPolyData = vtkSmartPointer<vtkPolyData>::New();
    pddPolyData->SetPoints(pddPoints);

    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType* ids = new vtkIdType[numPoints];
    for (int i = 0; i < numPoints; ++i) ids[i] = i;
    lines->InsertNextCell(numPoints, ids);
    delete[] ids;
    pddPolyData->SetLines(lines);

    vtkSmartPointer<vtkDoubleArray> pddScalars = vtkSmartPointer<vtkDoubleArray>::New();
    pddScalars->SetNumberOfComponents(1);
    pddScalars->SetName("Dosis");
    for (int i = 0; i < numPoints; ++i) {
        double z = -PHANTOM_HALF + i * (PHANTOM_SIZE / (numPoints-1));
        double dose = CalculateDose(z, 0.0, state.particle, state.energyMV);
        pddScalars->InsertNextValue(dose);
    }
    pddPolyData->GetPointData()->SetScalars(pddScalars);

    vtkSmartPointer<vtkPolyDataMapper> pddMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pddMapper->SetInputData(pddPolyData);
    pddMapper->SetLookupTable(state.doseLUT);
    pddMapper->ScalarVisibilityOn();

    vtkSmartPointer<vtkActor> pddActor = vtkSmartPointer<vtkActor>::New();
    pddActor->SetMapper(pddMapper);
    pddActor->GetProperty()->SetLineWidth(4);
    renderer->AddActor(pddActor);
    state.pddCurveActor = pddActor;

    // Actualizar texto informativo
    std::ostringstream oss;
    oss << "Partícula: " << state.particle << "  Energía: " << state.energyMV << " MV\n";
    oss << "Teclas: [F] Fotones [N] Neutrones [E] Electrones [P] Protones\n";
    oss << "        [6] 6MV  [0] 10MV  [5] 15MV  [2] 20MV";
    state.infoActor->SetInput(oss.str().c_str());
    renderer->AddActor(state.infoActor);

    renderer->Render();
}

// ============================================================
// Callback para teclado
// ============================================================
void KeypressCallback(vtkObject* caller, unsigned long eid, void* clientdata, void* calldata) {
    vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
    std::string key = interactor->GetKeySym();
    SimulationState* state = static_cast<SimulationState*>(clientdata);

    bool changed = false;
    if (key == "f" || key == "F") {
        state->particle = "foton";
        changed = true;
    } else if (key == "n" || key == "N") {
        state->particle = "neutron";
        changed = true;
    } else if (key == "e" || key == "E") {
        state->particle = "electron";
        changed = true;
    } else if (key == "p" || key == "P") {
        state->particle = "proton";
        changed = true;
    } else if (key == "6") {
        state->energyMV = 6.0;
        changed = true;
    } else if (key == "0") {
        state->energyMV = 10.0;
        changed = true;
    } else if (key == "5") {
        state->energyMV = 15.0;
        changed = true;
    } else if (key == "2") {
        state->energyMV = 20.0;
        changed = true;
    }

    if (changed) {
        UpdateScene(*state);
    }
}

// ============================================================
// Función principal de la simulación
// ============================================================
void runRadiotherapy() {
    SimulationState state;
    state.particle = "foton";
    state.energyMV = 6.0;

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.1, 0.2, 0.4);
    state.renderer = renderer;

    BuildScene(state);

    vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(renderer);
    window->SetSize(1200, 800);
    window->SetWindowName("Interacción de Radiación con Fantoma de Agua");
    window->Render();
    state.window = window;

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);
    state.interactor = interactor;

    vtkSmartPointer<vtkCallbackCommand> keyCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    keyCallback->SetCallback(KeypressCallback);
    keyCallback->SetClientData(&state);
    interactor->AddObserver(vtkCommand::KeyPressEvent, keyCallback);

    interactor->Start();
}