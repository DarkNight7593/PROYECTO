#include "Web.h"

// Decorador base que implementa IPagina y contiene un puntero a otro objeto IPagina
class WebDecorator : public MainWeb {
protected:
    MainWeb* web;
    DatabaseIterator* iterador;
public:
    WebDecorator()=default;
    WebDecorator(MainWeb* web_) : web(web_) {}
    void mostrarpagina() override {
        web->mostrarpagina();
    }
    void mostrarDetallesPelicula(Pelicula* pelicula) {
        cout << "Titulo: " << pelicula->titulo << endl;
        cout << "Sinopsis: " << pelicula->plot_synopsis << endl;
        cout << "Like: " << (pelicula->like ? "Si" : "No") << endl;
        cout << "Ver mas tarde: " << (pelicula->watch_later ? "Si" : "No") << endl;
        cout << "Opciones: (l) Like, (v) Ver mas tarde, (b) Volver: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'l') {
            pelicula->like = !pelicula->like;
            Sesion::Like(pelicula);
            limpiar(1);
            mostrarDetallesPelicula(pelicula);
        } else if (opcion == 'v') {
            pelicula->watch_later= !pelicula->watch_later;
            Sesion::VerMasTarde(pelicula);
            limpiar(1);
            mostrarDetallesPelicula(pelicula);
        }
        else if(opcion=='b') {
            return;
        }
        else mostrarDetallesPelicula(pelicula);
    }

    void resultados() {
        cout<<"Resultados:"<<endl;
        vector<Pelicula*> currentList = iterador->getCurrentList();
        int i=1;
        for_each(currentList.begin(),currentList.end(),[&](Pelicula* pelicula){
            cout<<i<<") "<<pelicula->titulo<<endl;
            i++;
        });
        cout << "Opciones: (n) siguiente, (p) anterior, (e) salir, (s) seleccionar: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'n' && iterador->hasNext()) {
            limpiar(1);
            iterador->next();
            resultados();
        } else if (opcion == 'p') {
            limpiar(1);
            iterador->previous();
            resultados();
        } else if (opcion == 'e') {
            return;
        } else if (opcion == 's') {
            cout << "Seleccione el numero de la pelicula: ";
            int seleccion;
            cin >> seleccion;
            if (seleccion > 0 && seleccion <= currentList.size()) {
                limpiar(1);
                mostrarDetallesPelicula(currentList[seleccion - 1]);
                limpiar(1);
                resultados();
            }
        }
    }
};


class listaDecorator : public WebDecorator{
public:
    listaDecorator()=default;
    listaDecorator(MainWeb* web):WebDecorator(web){}
    void mostrarpagina() override{
        iterador=new DatabaseIterator(sesion->mostrarVerMasTarde());
        cout<<"Ver mas tarde:"<<endl;
        resultados();
    }
};


class MenuDecorator : public WebDecorator {
public:
    MenuDecorator()=default;
    MenuDecorator(MainWeb* web):WebDecorator(web){}
    void mostrarpagina() override{
        web->mostrarpagina();
        cout<<"MENU:\n1)Busqueda\n2)Recomendados\n3)Ver mas tarde\n4)exit"<<endl;
    }
};



// Decorador concreto que añade paginación a los resultados de la búsqueda
class PaginacionDecorator : public WebDecorator {
private:
    bool tipo;
public:
    PaginacionDecorator()=default;
    PaginacionDecorator(MainWeb* web,bool tipo_):WebDecorator(web),tipo(tipo_){}

    void mostrarpagina() override {
        string palabraBuscada;
        cout << "Ingrese una palabra clave para buscar peliculas: ";
        cin >> palabraBuscada;
        iterador= new DatabaseIterator(web->buscarPelicula(palabraBuscada, tipo));
        resultados();
    }
};