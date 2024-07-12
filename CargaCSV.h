#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

// Estructura que representa una película
struct Pelicula {
    string imdb_id;
    string titulo;
    string plot_synopsis;
    string tags;
    string split;
    string synopsis_source;
    bool like;
    bool watch_later;
};

// Función para dividir una línea de CSV en sus componentes
vector<string> splitCSVLine(ifstream& archivo, string& linea) {
    vector<string> campos;
    string campo;
    bool dentroComillas = false;

    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == '"') {
            dentroComillas = !dentroComillas;
        } else if (c == ',' && !dentroComillas) {
            campos.push_back(campo);
            campo.clear();
        } else {
            campo += c;
        }
    }

    while (dentroComillas && getline(archivo, linea)) {
        campo += ' ';
        for (size_t i = 0; i < linea.size(); ++i) {
            char c = linea[i];
            if (c == '"') {
                dentroComillas = !dentroComillas;
            } else if (c == ',' && !dentroComillas) {
                campos.push_back(campo);
                campo.clear();
            } else {
                campo += c;
            }
        }
    }
    campos.push_back(campo);
    return campos;
}

vector<Pelicula> leerCSV(const string& nombreArchivo) {
    vector<Pelicula> peliculas;
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return peliculas;
    }

    string linea;
    // Leer la primera línea (cabeceras) y descartarla
    getline(archivo, linea);
    int i = 0;
    while (getline(archivo, linea)) {
        i += 1;
        vector<string> campos = splitCSVLine(archivo, linea);

        if (campos.size() == 6) {
            Pelicula pelicula = {
                    campos[0],   // imdb_id
                    campos[1],   // titulo
                    campos[2],   // plot_synopsis
                    campos[3],   // tags
                    campos[4],   // split
                    campos[5],   // synopsis_source
                    false,       // like
                    false        // watch_later
            };
            peliculas.push_back(pelicula);
        } else {
            cout << "Error en el formato de la línea: " << linea << endl;
        }
    }

    archivo.close();
    return peliculas;
}
