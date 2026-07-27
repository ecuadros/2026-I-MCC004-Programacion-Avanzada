#include "regexDemo.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void DemoRegex() {
    cout << "\n=== DEMOSTRACIÓN DE EXPRESIONES REGULARES ===\n";

    // ------------------------------------------------------------
    // Ejemplo 1: Validar un correo electrónico
    // ------------------------------------------------------------
    cout << "\n1. Validación de correo electrónico:\n";
    vector<string> emails = {
        "usuario@dominio.com",
        "nombre.apellido@sub.dominio.co",
        "invalido@sinpunto",
        "@sinusuario.com",
        "espacio @dominio.com"
    };

    regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");

    for (const auto& email : emails) {
        bool valido = regex_match(email, emailRegex);
        cout << (valido ? "[OK] " : "[NO] ") << email << endl;
    }

    // ------------------------------------------------------------
    // Ejemplo 2: Extraer fechas en formato DD/MM/YYYY
    // ------------------------------------------------------------
    cout << "\n2. Extracción de fechas (DD/MM/YYYY):\n";
    string texto = "Las fechas importantes son: 15/08/2024, 01/01/2025 y 31/12/2023.";
    regex fechaRegex(R"(\b(\d{2})/(\d{2})/(\d{4})\b)");
    smatch coincidencias;

    string::const_iterator start = texto.cbegin();
    while (regex_search(start, texto.cend(), coincidencias, fechaRegex)) {
        cout << "Fecha encontrada: " << coincidencias[0] 
             << " (día=" << coincidencias[1]
             << ", mes=" << coincidencias[2]
             << ", año=" << coincidencias[3] << ")" << endl;
        start = coincidencias.suffix().first;
    }

    // ------------------------------------------------------------
    // Ejemplo 3: Reemplazar todos los números por [NUM]
    // ------------------------------------------------------------
    cout << "\n3. Reemplazo de números por [NUM]:\n";
    string frase = "Tengo 2 manzanas y 3 peras, en total 5 frutas.";
    regex numeroRegex(R"(\d+)");
    string resultado = regex_replace(frase, numeroRegex, "[NUM]");
    cout << "Original: " << frase << endl;
    cout << "Reemplazada: " << resultado << endl;

    // ------------------------------------------------------------
    // Ejemplo 4: Buscar palabras que comiencen con 'c' (sin distinción)
    // ------------------------------------------------------------
    cout << "\n4. Palabras que comienzan con 'c' (insensible a mayúsculas):\n";
    string oracion = "Carlos come cerezas con crema y chocolate.";
    regex palabraCRegex(R"(\b[cC]\w*\b)");
    auto words_begin = sregex_iterator(oracion.begin(), oracion.end(), palabraCRegex);
    auto words_end = sregex_iterator();

    for (sregex_iterator it = words_begin; it != words_end; ++it) {
        cout << "- " << it->str() << endl;
    }

    cout << "\n=== FIN DE LA DEMOSTRACIÓN ===\n";
}