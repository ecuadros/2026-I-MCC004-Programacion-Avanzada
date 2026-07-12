#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main()
{
    // 1. VALIDAR UN CORREO ELECTRONICO
    string correo;

    cout << "=== VALIDACION DE CORREO ===" << endl;
    cout << "Ingrese un correo electronico: ";
    getline(cin, correo);

    regex patronCorreo(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
    );

    if (regex_match(correo, patronCorreo))
    {
        cout << "El correo tiene un formato valido." << endl;
    }
    else
    {
        cout << "El correo no tiene un formato valido." << endl;
    }

    // 2. BUSCAR UN NUMERO DENTRO DE UN TEXTO
    string texto;
    smatch coincidencia;

    cout << "\n=== BUSQUEDA DE NUMEROS ===" << endl;
    cout << "Ingrese un texto que contenga numeros: ";
    getline(cin, texto);

    regex patronNumero(R"([0-9]+)");

    if (regex_search(texto, coincidencia, patronNumero))
    {
        cout << "El primer numero encontrado es: "
             << coincidencia.str() << endl;
    }
    else
    {
        cout << "No se encontraron numeros en el texto." << endl;
    }

    // 3. REEMPLAZAR LOS NUMEROS DEL TEXTO
    cout << "\n=== REEMPLAZO DE NUMEROS ===" << endl;

    string textoModificado =
        regex_replace(texto, patronNumero, "[NUMERO]");

    cout << "Texto original: " << texto << endl;
    cout << "Texto modificado: " << textoModificado << endl;

    return 0;
}