#ifndef PROYECTO_H
#define PROYECTO_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

/**
 * @brief Ejecuta la simulación de interacción de radiación con fantoma de agua.
 * Permite cambiar partícula y energía mediante teclas.
 */
void runRadiotherapy();

#endif // PROYECTO_H