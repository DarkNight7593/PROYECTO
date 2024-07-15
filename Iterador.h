#include "Arbol_Nodo.h"
class DatabaseIterator {
private:
    vector<Pelicula*>* peliculas;
    size_t indice;
    vector<Pelicula*> currentList;
public:
    DatabaseIterator(){}
    DatabaseIterator(vector<Pelicula*>* resultados) : peliculas(resultados), indice(0) {}

    vector<Pelicula*>& getCurrentList() {
        currentList={};
        auto start = peliculas->begin() + indice;
        auto end = (indice + 5 < peliculas->size()) ? peliculas->begin() + indice + 5 : peliculas->end();
        currentList.assign(start, end);
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
        return indice + 5 < peliculas->size();
    }

    bool hasPrevious() {
        return indice > 0;
    }
};