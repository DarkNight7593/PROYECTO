#include "SesionCSV.h"
#include <cstdlib>
#include <windows.h>

void limpiar(int segundos){
    Sleep(segundos*1000);
    system("cls");
}
// Interfaz Web
class Web {
protected:
    vector<Pelicula*> peliculas;
    Trie trieTitulos;
    Trie trieTags;

public:
    Web()=default;
    virtual void accediendo() = 0;
    virtual void mostrarpagina(){}

    vector<Pelicula*> buscarPelicula(const string& key, bool buscarPorTitulo) {
        if (buscarPorTitulo) return trieTitulos.search(key);
        else return trieTags.search(key);
    }

    virtual ~Web() = default;
};

// Clase principal MainWeb que implementa la interfaz Web
class MainWeb : public Web {
public:
    Sesion* sesion;
    MainWeb()=default;

    void accediendo() override {
        cout << "Accediendo a la web......." << endl;
        string nombreArchivo = "mpst_full_data.csv";
        peliculas = leerCSV(nombreArchivo);
        for (auto& pelicula : peliculas) {
            trieTitulos.insert(pelicula->titulo, *pelicula);
            istringstream tagStream(pelicula->tags);
            string tag;
            while (getline(tagStream, tag, ',')) {
                trieTags.insert(tag, *pelicula);
            }
        }
        cout << "Accedio exitosamente" << endl;
        limpiar(2);
        iniciarsesion();
        limpiar(1);
    }
    void iniciarsesion(){
        cout<<"LOGIN:"<<endl;
        string username;
        string password;
        cout<<"Username: ";
        cin>>username;
        cout<<"Password: ";
        cin>>password;
        sesion=Sesion::iniciar(username,password);

        while (sesion == nullptr) {
            cerr << ("Error al iniciar sesion\nIntentalo de nuevo!") << endl;
            limpiar(3);
            cout<<"LOGIN:"<<endl;
            cout<<"Username: ";
            cin>>username;
            cout<<"Password: ";
            cin>>password;
            sesion=Sesion::iniciar(username,password);
        }
    }
    void mostrarpagina() override{
        cout << "Bienvenido a la plataforma de streaming" << endl;
    }
};

// Clase ProxyWeb que implementa la interfaz Web y controla el acceso a MainWeb
class ProxyWeb : public Web {
private:
    MainWeb* mainWeb;

public:
    bool PaisPermitido;
    bool TienePermiso;

    ProxyWeb() : PaisPermitido(false), TienePermiso(false) {
        mainWeb = new MainWeb();
    }

    ~ProxyWeb() override {
        delete mainWeb;
    }

    void verificarPermisos() {
        string pais;
        cout << "Ingrese su pais: " << endl;
        cin >> pais;
        PaisPermitido = (pais == "Peru");

        string id;
        cout << "Ingrese su ID: " << endl;
        cin >> id;
        TienePermiso = (id == "12345678");
    }

    void accediendo() override {
        verificarPermisos();

        if (PaisPermitido && TienePermiso) {
            mainWeb->accediendo();
        } else {
            cerr << ("Acceso denegado. Pais no permitido o permisos insuficientes.\nIntentalo de nuevo!") << endl;
            limpiar(3);
            accediendo();
        }
    }
    MainWeb* getMainWeb(){
        return mainWeb;
    }
};



