#include <iostream>
    using std::cout;
    using std::endl;
    using std::cin;
#include <string>
    using std::string;
    using std::getline;
#include <NTL/ZZ.h>
    using NTL::ZZ;
    using NTL::conv;
#include "RSA.h"

int main(){
    RSA receptor( 2048 );
    RSA emisor;
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
                cout << "Ingrese el mensaje cifrado: ";
                getline( cin, msgDescifrado );
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

