#include "Iterador.h"
#include "forward_list"
#include "algorithm"
class Sesion {
private:
    static Sesion* instance;
    static vector<Pelicula*>* watchLaterList;
    static forward_list<Pelicula*>* likedList;

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
        if(!pelicula->like) likedList->remove(pelicula);
        else{
            auto it=likedList->before_begin();
            for(int i=0;i<distance(likedList->begin(), likedList->end());i++) next(it);
            likedList->insert_after(it,pelicula);
        }
    }

    vector<Pelicula*>* mostrarVerMasTarde(){
        return watchLaterList;
    }
    void mostrarlikes(){
        cout << "Películas con likes:" << endl;
        for_each(likedList->begin(), likedList->end(), [](Pelicula* pelicula){cout << pelicula->titulo << endl;});
    }


};

// Inicializar la instancia de Sesion a nullptr
Sesion* Sesion::instance = nullptr;
forward_list<Pelicula*>* Sesion::likedList = new forward_list<Pelicula*>;
vector<Pelicula*>* Sesion::watchLaterList = new vector<Pelicula*>;