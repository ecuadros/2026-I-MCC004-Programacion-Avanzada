/**
 * @file proyecto.h
 * @brief Simulación de interacción de radiación con un fantoma de agua usando VTK.
 * @details Permite visualizar la distribución de dosis para diferentes partículas y energías.
 *          La interacción se realiza mediante teclas que cambian la partícula o la energía.
 * @author (Tu nombre o el del equipo)
 * @date 2026
 */

#ifndef PROYECTO_H
#define PROYECTO_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

/**
 * @brief Ejecuta la simulación de interacción de radiación con un fantoma de agua.
 * 
 * Crea una ventana VTK con la representación 3D de un fantoma cúbico de agua,
 * muestra la distribución de dosis como puntos coloreados, curvas de isodosis,
 * un corte axial y la curva PDD (Percentage Depth Dose). Permite cambiar
 * la partícula incidente (fotón, neutrón, electrón, protón) y la energía
 * (6, 10, 15, 20 MV) mediante teclas en tiempo real.
 * 
 * @note Las teclas de control se muestran en la esquina inferior izquierda.
 * @warning Requiere VTK instalado y correctamente enlazado.
 */
void runRadiotherapy();

#endif // PROYECTO_H