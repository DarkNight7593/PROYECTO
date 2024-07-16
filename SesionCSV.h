#include "Iterador.h"
#include "forward_list"
#include "algorithm"
class Sesion {
private:
    static Sesion* instance;
    static vector<Pelicula*>* watchLaterList;
    static vector<Pelicula*>* likedList;

    Sesion(){}

public:

    static Sesion* iniciar(const string& username, const string& password) {
        if (instance == nullptr && (username == "user" && password == "123")) instance = new Sesion();
        return instance;
    }

    void static VerMasTarde(Pelicula* pelicula) {
        if(!pelicula->watch_later) watchLaterList->erase(remove_if(watchLaterList->begin(), watchLaterList->end(), [&](Pelicula* peli) { return peli == pelicula; }), watchLaterList->end());
        else watchLaterList->emplace_back(pelicula);
    }

    void static Like(Pelicula* pelicula) {
        if(!pelicula->like) likedList->erase(remove_if(likedList->begin(), likedList->end(), [&](Pelicula* peli) { return peli == pelicula; }), likedList->end());
        else likedList->emplace_back(pelicula);
    }

    vector<Pelicula*>* mostrarVerMasTarde(){
        return watchLaterList;
    }
    vector<Pelicula*>* mostrarlikes(){
        return likedList;
    }


};

// Inicializar
Sesion* Sesion::instance = nullptr;
vector<Pelicula*>* Sesion::likedList = new vector<Pelicula*>;
vector<Pelicula*>* Sesion::watchLaterList = new vector<Pelicula*>;