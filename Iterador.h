#include "Arbol_Nodo.h"
class DatabaseIterator {
private:
    vector<Pelicula*> peliculas;
    size_t indice;
public:
    DatabaseIterator(){}
    DatabaseIterator(const vector<Pelicula*>& resultados) : peliculas(resultados), indice(0) {}

    vector<Pelicula*> getCurrentList() const {
        vector<Pelicula*> currentList;
        for (size_t i = indice; i < peliculas.size() && i < indice + 5; ++i) {
            currentList.push_back(peliculas[i]);
        }
        return currentList;
    }

    void next() {
        if (hasNext()) {
            indice += 5;
        }
    }

    void previous() {
        if (hasPrevious()) {
            if (indice >= 5) {
                indice -= 5;
            } else {
                indice = 0;
            }
        }
    }

    bool hasNext() {
        return indice + 5 < peliculas.size();
    }

    bool hasPrevious() {
        return indice > 0;
    }
};