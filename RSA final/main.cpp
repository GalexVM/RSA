#include <iostream>
    using std::cout;
    using std::endl;
    using std::cin;
#include <string>
    using std::string;
    using std::getline;
#include <cstdlib>
    using std::ios;
#include <fstream>
    using std::ifstream;
#include "RSA.h"

string lectura();

int main(){
    RSA receptor( 1024, 0 );
    RSA emisor( 1024 );
    bool finSesion = false;
    cout << "Algoritmo RSA" << endl;
    cout << "Sus claves se han generado en el directorio local\n";

    while( finSesion == false ) {
        string msg;
        string msgDescifrado;
        cout << "Para cifrar un mensaje, presione c\n";
        cout << "Para descifrar un mensaje, presione d\n";
        cout << "Para cerrar sesion, presione s\n";
        char key;
        cout << " : ";
        cin >> key;
        cin.ignore();

        switch( key ){
        case 'c':
                cout << "Ingrese el mensaje a cifrar: ";
                getline( cin, msg );
                cout << emisor.cifrar( msg ) << endl;

            break;
        case 'd':
                cout << "Leyendo mensaje cifrado guardado...\n";
                msgDescifrado = lectura();
                cout << receptor.descifrar( msgDescifrado ) << endl;
            break;
        case 's':
            finSesion = true;
            break;
        default:
            cout << "Opcion no valida\n";
        }
    }
    return 0;
}

string lectura(){
    ifstream archivo;
    string texto, resultado;
    archivo.open("cifrado.txt",ios::in);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo de texto cifrado\n";
        exit(1);
    }
    while(!archivo.eof()){
        getline(archivo,texto);
        resultado += texto;
    }
    return resultado;
}
