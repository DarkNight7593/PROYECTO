#include "SesionCSV.h"
// Interfaz base para todas las decoraciones de la página
class IPagina {
public:
    virtual ~IPagina() = default;
    virtual void mostrar() const = 0;
};

// Clase concreta que representa el contenido base de la página
class PaginaBase : public IPagina {
public:
    PaginaBase(){}
    void mostrar() const override {
        cout << "Bienvenido a la plataforma de streaming" << endl;
    }
};

// Decorador base que implementa IPagina y contiene un puntero a otro objeto IPagina
class PaginaDecorator : public IPagina {
protected:
    IPagina* pagina;

public:
    PaginaDecorator()=default;
    PaginaDecorator(IPagina* pagina) : pagina(pagina) {}
    void mostrar() const override {
        pagina->mostrar();
    }
};


// Decorador concreto que añade paginación a los resultados de la búsqueda
class PaginacionDecorator : public PaginaDecorator {
private:
    Sesion* sesion;
public:
    PaginacionDecorator()=default;
    PaginacionDecorator(IPagina* pagina, Sesion* sesion): PaginaDecorator(pagina), sesion(sesion) {
        string palabraBuscada;
        cout << "Ingrese una palabra clave para buscar peliculas: ";
        getline(cin, palabraBuscada);
        sesion->setIterator(DatabaseIterator(sesion->buscarPelicula(palabraBuscada, true)));
    }

    void mostrar() const override {
        PaginaDecorator::mostrar();
        mostrarPagina();
    }

    void mostrarPagina() const {
        cout << "Resultados de la busqueda:" << endl;
        vector<Pelicula> currentList = sesion->getIterator().getCurrentList();
        for (size_t i = 0; i < currentList.size(); ++i) {
            cout << i + 1 << ". Titulo: " << currentList[i].titulo << endl;
        }

        cout << "Opciones: (n) siguiente, (p) anterior, (e) salir, (s) seleccionar: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'n' && sesion->getIterator().hasNext()) {
            sesion->getIterator().next();
            mostrarPagina();
        } else if (opcion == 'p' && sesion->getIterator().hasPrevious()) {
            sesion->getIterator().previous();
            mostrarPagina();
        } else if (opcion == 'e') {
            return;
        } else if (opcion == 's') {
            cout << "Seleccione el número de la película: ";
            int seleccion;
            cin >> seleccion;
            if (seleccion > 0 && seleccion <= currentList.size()) {
                mostrarDetallesPelicula(currentList[seleccion - 1]);
            }
        } else {
            mostrarPagina();
        }
    }

    void mostrarDetallesPelicula(Pelicula& pelicula) const {
        cout << "Titulo: " << pelicula.titulo << endl;
        cout << "Sinopsis: " << pelicula.plot_synopsis << endl;
        cout << "Like: " << (pelicula.like ? "Sí" : "No") << endl;
        cout << "Ver más tarde: " << (pelicula.watch_later ? "Sí" : "No") << endl;
        cout << "Opciones: (l) Like, (v) Ver más tarde, (b) Volver: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'l') {
            pelicula.like = !pelicula.like;
            sesion->agregarLike(pelicula);
        } else if (opcion == 'v') {
            pelicula.watch_later= !pelicula.watch_later;
            sesion->agregarVerMasTarde(pelicula);
        }
        mostrarPagina();
    }
};
int main() {
    // Iniciar sesión
    string username = "user";
    string password = "123";
    Sesion* sesion = Sesion::iniciar(username, password);

    if (sesion == nullptr) {
        cout << "Error al iniciar sesion" << endl;
        return 1;
    }

    // Crear y mostrar la página con decoradores
    IPagina* pagina = new PaginaBase();
    pagina = new PaginacionDecorator(pagina, sesion);

    pagina->mostrar();

    delete pagina;

    return 0;
}
