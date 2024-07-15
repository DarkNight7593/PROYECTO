#include "Iterador.h"
#include "forward_list"
#include "algorithm"
class Sesion {
private:
    static Sesion* instance;
    static forward_list<Pelicula*>* watchLaterList;
    static forward_list<Pelicula*>* likedList;

    Sesion(){}

public:

    static Sesion* iniciar(const string& username, const string& password) {
        if (instance == nullptr && (username == "user" && password == "123")) instance = new Sesion();
        return instance;
    }

    void static VerMasTarde(Pelicula* pelicula) {
        if(!pelicula->watch_later) watchLaterList->remove(pelicula);
        else{
            auto it=watchLaterList->before_begin();
            for(int i=0;i<distance(watchLaterList->begin(), watchLaterList->end());i++) next(it);
            watchLaterList->insert_after(it,pelicula);
        }
    }

    void static Like(Pelicula* pelicula) {
        if(!pelicula->like) likedList->remove(pelicula);
        else{
            auto it=likedList->before_begin();
            for(int i=0;i<distance(likedList->begin(), likedList->end());i++) next(it);
            likedList->insert_after(it,pelicula);
        }
    }

    vector<Pelicula*> mostrarVerMasTarde(){
        vector<Pelicula*> lista;
        for_each(watchLaterList->begin(), watchLaterList->end(), [&](Pelicula* pelicula){lista.emplace_back(pelicula);});
        return lista;
    }
    void mostrarlikes(){
        cout << "Películas con likes:" << endl;
        for_each(likedList->begin(), likedList->end(), [](Pelicula* pelicula){cout << pelicula->titulo << endl;});
    }


};

// Inicializar la instancia de Sesion a nullptr
Sesion* Sesion::instance = nullptr;
forward_list<Pelicula*>* Sesion::likedList = new forward_list<Pelicula*>;
forward_list<Pelicula*>* Sesion::watchLaterList = new forward_list<Pelicula*>;