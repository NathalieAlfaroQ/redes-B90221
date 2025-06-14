// Bibliotecas
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;

// Estructura para guardar la info de cada bloque
struct Bloque {
    // Nombre del bloque: A, B...
    string nombreBloque;
    // Direcciones iniciales de cada bloque
    int solicitudesPorBloque;
    // Direcciones a asignar después de sumarle 2
    int direccionesAsignadas;
    int prefijo;
    // Dirección de la red en int grande
    uint32_t red;

    // Calcula la dirección tipo broadcast
    uint32_t direccionBroadcast() const {
        return red + direccionesAsignadas - 1;
    }

    // Calcula la primera dirección que se puede utilizar
    uint32_t primeraDireccionUtil() const {
        return red + 1;
    }

    // Calcula la última dirección que se puede utilizar
    uint32_t ultimaDireccionUsable() const {
        return direccionBroadcast() - 1;
    }
};

// Convierte la dirección base de string a int
uint32_t direccionIP_a_Int(string& ip) {
    int a, b, c, d;
    sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// Convierte la IP que está en un número completo al formato usual de una IP separada de puntos
string IPformato(uint32_t ip) {
    return to_string((ip >> 24) & 0xFF) + "." +
           to_string((ip >> 16) & 0xFF) + "." +
           to_string((ip >> 8) & 0xFF) + "." +
           to_string(ip & 0xFF);
}

// Convierte un prefijo a máscara 
string mascaraBloque(int prefijo) {
    uint32_t mascara = 0xFFFFFFFF << (32 - prefijo); 
    return to_string((mascara >> 24) & 0xFF) + "." +
           to_string((mascara >> 16) & 0xFF) + "." +
           to_string((mascara >> 8) & 0xFF) + "." +
           to_string(mascara & 0xFF);
}

// Calcula la cantidad de direcciones que puede usar cada blowque
int calcularPrefijo(int cantidad) {
    // Direcciones del bloque + 2
    int total = cantidad + 2;
    // Busca la potencia de 2 adecuada para el resultado de la suma
    int bits = ceil(log2(total));
    return 32 - bits;
}

// Calcula la cantidad de direcciones definitivas que llevará cada bloque
int tamanoBloque(int prefijo) {
    return pow(2, 32 - prefijo);
}

// Ordena los bloques de mayor a menor
bool ordenaBloquesAscendente(Bloque& a, Bloque& b) {
    return a.direccionesAsignadas > b.direccionesAsignadas;
}

int main(int argc, char* argv[]) {
    // Se pasa un archivo de texto con la entrada
    // Contiene la dirección base a trabajar y los bloques a ordenar y asignar direcciones
    // Intenta abrir el archivo
    ifstream archivo("prueba.txt");

    // Mensaje de error por si no se puede abrir el archivo
    if (!archivo) {
        cerr << "No se pudo abrir el archivo.\n";
        return 1;
    }

    // Guarda la info del archivo en un string
    string linea;
    // Lee el archivo
    getline(archivo, linea);
    // Llama la función que convierte de string a int
    uint32_t ipBase = direccionIP_a_Int(linea);
    // Guardaremos los bloques en un vector
    vector<Bloque> bloques;

    // Se empieza a leer las siguientes líneas con los bloques del archivo
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        int cantidad;
        string temp, nombreBloque;
        ss >> cantidad >> temp >> temp >> nombreBloque;
        
        // A cada bloque se le calculan sus datos y se suben al vector
        Bloque bloque;
        bloque.nombreBloque = nombreBloque;
        bloque.solicitudesPorBloque = cantidad;
        bloque.prefijo = calcularPrefijo(cantidad);
        bloque.direccionesAsignadas = tamanoBloque(bloque.prefijo);
        // Subiendo al vector
        bloques.push_back(bloque);
    }

    // Se llama a la función ordenar y se le pasa todo el vector
    // con la función que ordena de forma ascendente
    sort(bloques.begin(), bloques.end(), ordenaBloquesAscendente);

    // Ciclo para imprimir los bloques con su análisis
    for (int i = 0; i < bloques.size(); i++) {
        Bloque& b = bloques[i];
        // Indicación de emepzar con la red base
        b.red = ipBase;

        // Imprime las direcciones por categoría de cada bloque
        cout << "Bloque " << b.nombreBloque << endl;
        cout << "Network: " << IPformato(b.red) << endl;
        cout << "Usables: " << IPformato(b.primeraDireccionUtil()) << " a " << IPformato(b.ultimaDireccionUsable()) << endl;
        cout << "Broadcast: " << IPformato(b.direccionBroadcast()) << endl;
        cout << "Máscara: " << mascaraBloque(b.prefijo) << endl << endl;

        // Siguiente bloque
        ipBase += b.direccionesAsignadas;
    }

    // Fin del programa
    return 0;
}