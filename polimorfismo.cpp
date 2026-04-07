#include <iostream>
#include <vector>
#include <memory>
#include "rectangle.h"
#include "circle.h"
#include "triangle.h"

using namespace std;

void DemoPolimorfismo() {

    // Crear 3 objetos bajo punteros de tipo Shape
    Shape *sv[] = {new Rectangle("RectX", 4.0, 5.0), 
                   new Circle   ("CírcX", 3.0),
                   new Triangle ("TriaX", 6.0, 2.0)
                };
    for(int i = 0; i < 3; i++){
        cout << sv[i]->GetName() 
             << " -> área: " << sv[i]->GetArea() 
             << endl;
    }

    for(int i = 0; i < 3; i++){
        delete sv[i];
    }

    // Crear un vector de punteros a Shape
    vector<unique_ptr<Shape>> formas;

    // Agregar figuras con nombre y dimensiones
    formas.push_back(make_unique<Rectangle>("Mi Rectángulo", 4.0, 5.0));
    formas.push_back(make_unique<Circle>("Mi Círculo", 3.0));
    formas.push_back(make_unique<Triangle>("Mi Triángulo", 6.0, 2.0));

    // Recorrer y mostrar nombre y área usando polimorfismo
    cout << "=== Demostración de polimorfismo ===" << endl;
    for (const auto& forma : formas) {
        // Se llama a GetName() de Shape y GetArea() virtual
        cout << forma->GetName() << " -> área: " << forma->GetArea() << endl;
    }
}