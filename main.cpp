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
    RSA receptor( 64, 0 );
    RSA emisor( 64 );
    bool finSesion = false;
    vector < string > res;
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
                res = emisor.cifrar( msg );
                cout << res[0] << endl;
                cout << res[1] << endl;
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

