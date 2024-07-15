#include "Web.h"
#include <thread>
#include <future>
#include <mutex>

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

class MegustaDecorator : public WebDecorator{
public:
    MegustaDecorator()=default;
    MegustaDecorator(MainWeb* web):WebDecorator(web){}
    void mostrarpagina() override{
        iterador=new DatabaseIterator(sesion->mostrarlikes());
        cout<<"Peliculas que me gustan:"<<endl;
        resultados();
    }
};


class MenuDecorator : public WebDecorator {
public:
    MenuDecorator()=default;
    MenuDecorator(MainWeb* web):WebDecorator(web){}
    void mostrarpagina() override{
        web->mostrarpagina();
        cout<<"MENU:\n1)Busqueda\n2)Recomendados\n3)Likes\n4)Ver mas tarde\n5)exit"<<endl;
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

class RecomendacionDecorator : public WebDecorator {
public:
    RecomendacionDecorator() = default;
    RecomendacionDecorator(MainWeb* web) : WebDecorator(web) {}

    void mostrarpagina() override {
        vector<Pelicula*> likedMovies = *(sesion->mostrarlikes());
        unordered_map<string, int> tagFrequency;

        // Contar la frecuencia de los tags en las películas que les gustaron al usuario
        for (auto& pelicula : likedMovies) {
            istringstream tagStream(pelicula->tags);
            string tag;
            while (getline(tagStream, tag, ',')) {
                tagFrequency[tag]++;
            }
        }

        // Mapa para almacenar las películas y sus similarityScores
        unordered_map<Pelicula*, int> recommendedMoviesMap;
        vector<Pelicula*> recommendedMovies;

        for (auto& pelicula : web->getpeliculas()) {
            if (find(likedMovies.begin(), likedMovies.end(), pelicula) == likedMovies.end()) {
                int similarityScore = 0;
                istringstream tagStream(pelicula->tags);
                string tag;
                while (getline(tagStream, tag, ',')) {
                    similarityScore += tagFrequency[tag];
                }
                if (similarityScore > 0) {
                    recommendedMoviesMap[pelicula] = similarityScore;
                    recommendedMovies.push_back(pelicula);
                }
            }
        }

        // Crear promesas y futuros para el ordenamiento en subrangos
        size_t numThreads = 3;
        size_t chunkSize = recommendedMovies.size() / numThreads;
        vector<promise<void>> promises(numThreads);
        vector<future<void>> futures;

        for (auto& promise : promises) {
            futures.push_back(promise.get_future());
        }

        // Ordenar cada subrango en un hilo separado
        vector<thread> threads;
        for (size_t i = 0; i < numThreads; ++i) {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? recommendedMovies.size() : start + chunkSize;

            threads.emplace_back([start, end, &recommendedMovies, &recommendedMoviesMap, &promises, i]() {
                sort(recommendedMovies.begin() + start, recommendedMovies.begin() + end, [&](Pelicula* a, Pelicula* b) {
                    return recommendedMoviesMap[a] > recommendedMoviesMap[b];
                });
                promises[i].set_value();
            });
        }

        // Esperar a que todos los hilos terminen
        for (auto& future : futures) {
            future.get();
        }

        // Unir todos los hilos
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // Fusionar los subrangos ordenados
        vector<Pelicula*> sortedMovies;
        vector<size_t> indices(numThreads, 0);
        for (size_t i = 0; i < numThreads; ++i) {
            indices[i] = i * chunkSize;
        }

        while (true) {
            Pelicula* minPelicula = nullptr;
            size_t minIndex = 0;
            for (size_t i = 0; i < numThreads; ++i) {
                if (indices[i] < ((i == numThreads - 1) ? recommendedMovies.size() : (i + 1) * chunkSize)) {
                    if (!minPelicula || recommendedMoviesMap[recommendedMovies[indices[i]]] > recommendedMoviesMap[minPelicula]) {
                        minPelicula = recommendedMovies[indices[i]];
                        minIndex = i;
                    }
                }
            }
            if (!minPelicula) break;
            sortedMovies.push_back(minPelicula);
            indices[minIndex]++;
        }

        iterador = new DatabaseIterator(&sortedMovies);
        resultados();
    }
};