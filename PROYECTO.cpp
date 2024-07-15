#include "Decorator.h"
void opcioninvalida(){
    cerr << ("Opcion no valida. \nIntentalo de nuevo!!") << endl;
    limpiar(2);
}
void funcion(MenuDecorator* Menu,PaginacionDecorator* busquedatitulo,PaginacionDecorator* busquedatag,listaDecorator* lista, RecomendacionDecorator* recomendacion,MegustaDecorator* megusta){
    Menu->mostrarpagina();
    int option;
    cin>>option;
    while (option>5 || option<1){
        opcioninvalida();
        Menu->mostrarpagina();
        cin>>option;
    }
    limpiar(1);
    if(option==1){
        cout<<"1)Por titulo\n2)Por tag"<<endl;
        cin>>option;
        while (option>2 || option<1){
            opcioninvalida();
            cout<<"1)Por titulo\n2)Por tag"<<endl;
            cin>>option;
        }
        limpiar(1);
        if(option==1) busquedatitulo->mostrarpagina();
        else busquedatag->mostrarpagina();
        limpiar(1);
        funcion(Menu,busquedatitulo,busquedatag,lista,recomendacion,megusta);
    }
    else if(option==2){
        recomendacion->mostrarpagina();
        limpiar(1);
        funcion(Menu,busquedatitulo,busquedatag,lista,recomendacion,megusta);
    }
    else if(option==3){
        limpiar(1);
        megusta->mostrarpagina();
        limpiar(1);
        funcion(Menu,busquedatitulo,busquedatag,lista,recomendacion,megusta);
    }
    else if(option==4){
        limpiar(1);
        lista->mostrarpagina();
        limpiar(1);
        funcion(Menu,busquedatitulo,busquedatag,lista,recomendacion,megusta);
    }
    else if(option==5){
        return;
    }
}
int main() {
    ProxyWeb* web=new ProxyWeb();
    web->accediendo();
    MenuDecorator* Menu=new MenuDecorator(web->getMainWeb());
    PaginacionDecorator* busquedatitulo=new PaginacionDecorator(web->getMainWeb(), true);
    PaginacionDecorator* busquedatag=new PaginacionDecorator(web->getMainWeb(), false);
    listaDecorator* lista=new listaDecorator(web->getMainWeb());
    MegustaDecorator* megusta=new MegustaDecorator(web->getMainWeb());
    RecomendacionDecorator* recomendacion = new RecomendacionDecorator(web->getMainWeb());
    funcion(Menu,busquedatitulo,busquedatag,lista,recomendacion,megusta);
    cout<<"GRACIAS POR USAR NUESTRA PLATAFORMA"<<endl;
    return 0;
}
