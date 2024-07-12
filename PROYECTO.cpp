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
    void mostrar() const override {
        cout << "Bienvenido a la plataforma de streaming" << endl;
    }
};

// Decorador base que implementa IPagina y contiene un puntero a otro objeto IPagina
class PaginaDecorator : public IPagina {
protected:
    IPagina* pagina;

public:
    PaginaDecorator(IPagina* pagina) : pagina(pagina) {}
    void mostrar() const override {
        pagina->mostrar();
    }
};

// Decorador concreto que añade un buscador a la página
class BuscadorDecorator : public PaginaDecorator {
public:
    BuscadorDecorator(IPagina* pagina) : PaginaDecorator(pagina) {}

    void mostrar() const override {
        PaginaDecorator::mostrar();
        mostrarBuscador();
    }

    void mostrarBuscador() const {
        cout << "Ingrese una palabra clave para buscar películas: " << endl;
    }
};

// Decorador concreto que muestra los primeros cinco resultados de la búsqueda
class ResultadosDecorator : public PaginaDecorator {
private:
    vector<Pelicula> resultados;

public:
    ResultadosDecorator(IPagina* pagina, const vector<Pelicula>& resultados)
            : PaginaDecorator(pagina), resultados(resultados) {}

    void mostrar() const override {
        PaginaDecorator::mostrar();
        mostrarResultados();
    }

    void mostrarResultados() const {
        cout << "Resultados de la búsqueda:" << endl;
        for (size_t i = 0; i < resultados.size() && i < 5; ++i) {
            cout << i + 1 << ". Título: " << resultados[i].titulo << endl;
        }
    }
};

// Decorador concreto que añade paginación a los resultados de la búsqueda
class PaginacionDecorator : public PaginaDecorator {
private:
    DatabaseIterator& iterator;
    Sesion* sesion;

public:
    PaginacionDecorator(IPagina* pagina, DatabaseIterator& iterator, Sesion* sesion)
            : PaginaDecorator(pagina), iterator(iterator), sesion(sesion) {}

    void mostrar() const override {
        PaginaDecorator::mostrar();
        mostrarPagina();
    }

    void mostrarPagina() const {
        vector<Pelicula> currentList = iterator.getCurrentList();
        for (size_t i = 0; i < currentList.size(); ++i) {
            cout << i + 1 << ". Título: " << currentList[i].titulo << endl;
        }

        cout << "Opciones: (n) siguiente, (p) anterior, (e) salir, (s) seleccionar: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'n' && iterator.hasNext()) {
            iterator.next();
            mostrarPagina();
        } else if (opcion == 'p' && iterator.hasPrevious()) {
            iterator.previous();
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

    void mostrarDetallesPelicula(const Pelicula& pelicula) const {
        cout << "Título: " << pelicula.titulo << endl;
        cout << "Sinopsis: " << pelicula.plot_synopsis << endl;
        cout << "Like: " << (pelicula.like ? "Sí" : "No") << endl;
        cout << "Ver más tarde: " << (pelicula.watch_later ? "Sí" : "No") << endl;
        cout << "Opciones: (l) Like, (v) Ver más tarde, (b) Volver: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'l') {
            sesion->agregarLike(pelicula);
        } else if (opcion == 'v') {
            sesion->agregarVerMasTarde(pelicula);
        }
        mostrarPagina();
    }
};
