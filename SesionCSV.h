#include "Iterador.h"
class Sesion {
private:
    static Sesion* instance;
    Trie trieTitulos;
    Trie trieTags;
    vector<Pelicula> peliculas;
    vector<Pelicula> watchLaterList;
    vector<Pelicula> likedList;
    DatabaseIterator iterator;

    // Constructor privado
    Sesion(const string& username, const string& password) {
        if (username == "user" && password == "123") {
            string nombreArchivo = "mpst_full_data.csv";
            peliculas = leerCSV(nombreArchivo);
            for (const auto& pelicula : peliculas) {
                trieTitulos.insert(pelicula.titulo, pelicula);
                istringstream tagStream(pelicula.tags);
                string tag;
                while (getline(tagStream, tag, ',')) {
                    trieTags.insert(tag, pelicula);
                }
            }
        }
    }

public:
    void setIterator(const DatabaseIterator &iterator_){
        iterator = iterator_;
    }

    DatabaseIterator& getIterator() {
        return iterator;
    }

    static Sesion* iniciar(const string& username, const string& password) {
        if (instance == nullptr) instance = new Sesion(username, password);
        return instance;
    }

    vector<Pelicula> buscarPelicula(const string& key, bool buscarPorTitulo) {
        if (buscarPorTitulo) return trieTitulos.search(key);
        else return trieTags.search(key);
    }

    void agregarVerMasTarde(const Pelicula& pelicula) {
        watchLaterList.push_back(pelicula);
    }

    void agregarLike(const Pelicula& pelicula) {
        likedList.push_back(pelicula);
    }

    void mostrarVerMasTarde() const {
        cout << "Películas en Ver más tarde:" << endl;
        for (const auto& pelicula : watchLaterList) {
            cout << "Título: " << pelicula.titulo << endl;
        }
    }

    void mostrarLikes() const {
        cout << "Películas que te gustan:" << endl;
        for (const auto& pelicula : likedList) {
            cout << "Título: " << pelicula.titulo << endl;
        }
    }
};

// Inicializar la instancia de Sesion a nullptr
Sesion* Sesion::instance = nullptr;
