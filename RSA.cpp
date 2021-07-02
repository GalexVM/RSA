#include <iostream>
    using std::cout;
    using std::endl;
    using std::cin;
#include <fstream>
    using std::ofstream;
#include <string>
    using std::string;
#include <NTL/ZZ.h>
    using NTL::ZZ;
    using NTL::conv;
#include <sstream>
    using std::ostringstream;
    using std::istringstream;
#include <vector>
    using std::vector;
#include "math-functions.h"
#include "RSA.h"


RSA::RSA( int bits ){
    firma = "Gabriel Alexander Valdivia Medina 201 10 47904 73127083 Dayevska Anabel Caceres Budiel 201 10 47457 73333523 Sergio Leandro Ramos Villena  201-10-47800 75974804 Roberto Juan Cayro Cuadros 201 10 47988 71508951 ";
    p = generarPrimoNTL( bits );
        cout<<". ";
    q = generarPrimoNTL( bits );
        cout<<". ";
    while( q == p )
        q = generarPrimoNTL( bits );
    N = p * q;
    ZZ oN ( ( p - 1 ) * ( q - 1 ) );
    e = generarCoprimoNTL( oN, bits );
        cout<<".\n";
    d = mod( inversa_modularNTL( e, oN ),oN );

    ostringstream salida;//Enviar claves al txt "claves_emisor"
    salida <<e<<"\t"<<N<<"\t"<<endl;
    ofstream out("claves_emisor.txt");
        out<<salida.str()<<endl;
    out.close();
}
RSA::RSA( int bits, int a ) {
    cout << "Generando claves\n";
    p = generarPrimoNTL( bits );
        cout<<". ";
    q = generarPrimoNTL( bits );
    while( q == p ){
        q = generarPrimoNTL( bits );}
        cout << ". ";
    N = p * q;
    ZZ oN ( ( p - 1 ) * ( q - 1 ) );
    e = generarCoprimoNTL( oN, bits );
        cout<<".\n";
    d = mod( inversa_modularNTL( e, oN ),oN );

    ostringstream salida;//Enviar claves al txt "claves_receptor"
    salida <<e<<"\t"<<N<<"\t"<<endl;
    ofstream out("claves_receptor.txt");
        out<<salida.str()<<endl;
    out.close();
}

 string  RSA::cifrar( string &msg ) {
    ZZ ePublico;
    ZZ NPublico;
    cout << "Ingrese la clave pública e de su destino: ";
    cin >> ePublico;
    cout << "\nIngrese la clave pública N de su destino: ";
    cin >> NPublico;
    cout << endl;

    //Cifrar mensaje.
    string msgConvertido =  alfabetoANumeros ( msg, alfabeto, NPublico );

    vector < string > vectorBloques ( separarBloques( msgConvertido, hallarDigitos(NPublico), NPublico) );

    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, ePublico, NPublico ), NPublico ) );//Exponenciar bloque por bloque.

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( NPublico ) - hallarDigitos( valorCifrado ) ); j++ )//Añadir 0 a la izq de ser necesario.
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string msgCifrado;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        msgCifrado += vectorBloques[ i ];
    }

    //Primer cifrado firma.
    string firmaConvertida =  alfabetoANumeros ( firma, alfabeto, N );

    vectorBloques = separarBloques( firmaConvertida, hallarDigitos(N), N );

    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, d, N ), N ) );//Exponenciar bloque por bloque.

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( N ) - hallarDigitos( valorCifrado ) ); j++ )//Añadir 0 a la izq de ser necesario.
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string firmaCifrada;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        firmaCifrada += vectorBloques[ i ];
    }

    //Segundo cifrado firma
    vectorBloques = separarBloques( firmaCifrada, hallarDigitos(NPublico), NPublico );

    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, ePublico, NPublico ), NPublico ) );//Cifrar bloque por bloque.

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( NPublico ) - hallarDigitos( valorCifrado ) ); j++ )//Agregar 0 a la izq de ser necesario.
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string firmaFinal;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        firmaFinal += vectorBloques[ i ];
    }

    ofstream out( "cifrado.txt" );
        out << msgCifrado << "-" << firmaFinal << endl;//Enviar ambos cifrados al txt "cifrado".
    out.close();

    string resultado;
    resultado = msgCifrado + " " + firmaFinal;//Crear string resultado.

    return resultado;

}

string RSA::descifrar( string &msg ) {
    string firm;
    string msg0;
    bool cambioClave = false;
    for( unsigned int i = 0; i < msg.size(); i++ ){//Dividir string en mensaje y firma.
        if( cambioClave == true ){
            firm.push_back( msg[ i ] );
        }else{
            if( msg[ i ] != '-')
                msg0.push_back( msg[ i ] );
        }
        if( msg[ i ] == '-' )
            cambioClave = true;
    }

    //Recibir clave pública del emisor.
    ZZ ePublico;
    ZZ NPublico;
    cout << "Ingrese la clave pública e de su remitente: ";
    cin >> ePublico;
    cout << "\nIngrese la clave pública N de su remitente: ";
    cin >> NPublico;
    cout << endl;

    //Descifrar mensaje.
    vector< string > vectorBloques = dividirBloques( N, msg0, msg0.size() / hallarDigitos( N ), hallarDigitos( N ) );

    for( int i = 0; i < vectorBloques.size(); i++ ){//Bloque por bloque...
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );
        ZZ valorDescifrado ( TRC( valorConvertido, d, p, q ) );//Teorema del Resto Chino

        ostringstream valoresNuevos;
         for( int j = 0; j < ( hallarDigitos( N ) - 1 - hallarDigitos( valorDescifrado ) ); j++ )//Regresar a string añaidendo 0s a la izq
            valoresNuevos << "0";
        valoresNuevos << valorDescifrado;
        vectorBloques[ i ] = valoresNuevos.str();//Reemplazar en vector
    }
    string msg2;
    for( int i = 0; i < vectorBloques.size(); i++ )//Resultado numérico.
        msg2 += vectorBloques[ i ];

    //Primer descifrado firma.
    vector< string > vectorBloques2 = dividirBloques( N, firm, firm.size() / hallarDigitos( N ), hallarDigitos( N ) );

    for( int i = 0; i < vectorBloques2.size(); i++ ){//Bloque por bloque...
        ZZ valorConvertido ( string_a_ZZ( vectorBloques2[ i ] ) );
        ZZ valorDescifrado ( TRC( valorConvertido, d, p, q ) );//Teorema del Resto Chino

        ostringstream valoresNuevos;
         for( int j = 0; j < ( hallarDigitos( N ) - 1 - hallarDigitos( valorDescifrado ) ); j++ )//Regresar a string añaidendo 0s a la izq
            valoresNuevos << "0";
        valoresNuevos << valorDescifrado;
        vectorBloques2[ i ] = valoresNuevos.str();//Reemplazar en vector
    }
    string Dr;

    for( int i = 0; i < vectorBloques2.size(); i++ )//Resultado numérico.
        Dr += vectorBloques2[ i ];

    //Segundo descifrado de firma
    vector< string > vectorBloques3 = dividirBloques( NPublico, Dr, Dr.size() / (hallarDigitos( NPublico )), hallarDigitos( NPublico ) );

    for( int i = 0; i < vectorBloques3.size(); i++ ){//Bloque por bloque...
        ZZ valorConvertido ( string_a_ZZ( vectorBloques3[ i ] ) );
        //ZZ valorDescifrado ( TRC( valorConvertido, d, p, q ) );//Teorema del Resto Chino
        ZZ valorDescifrado ( Binary_Exponentiation( valorConvertido, ePublico, NPublico ) );

        ostringstream valoresNuevos;
         for( int j = 0; j < ( hallarDigitos( NPublico ) -1- hallarDigitos( valorDescifrado ) ); j++ )//Regresar a string añaidendo 0s a la izq
            valoresNuevos << "0";
        valoresNuevos << valorDescifrado;
        vectorBloques3[ i ] = valoresNuevos.str();//Reemplazar en vector
    }
    string Df;
    for( int i = 0; i < vectorBloques3.size(); i++ )//Resultado numérico.
        Df += vectorBloques3[ i ];


    //Convertir a letras.
    vectorBloques.clear();
    int numeroBloques = msg0.size() / hallarDigitos( ZZ( alfabeto.size() - 1 ) );//Crear bloques de 2 (cifra significativa alfabeto)
    vectorBloques = dividirBloques( N, msg2, numeroBloques, hallarDigitos( ZZ( alfabeto.size() - 1 ) ) );

    for( int i = 0; i < vectorBloques.size(); i++ ){//Eliminar espacios sobrantes.
        if( vectorBloques[ i ] == "" )
            vectorBloques[ i ] = "26";
    }

    string resultados;//Resultado en letras del mensaje.
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Intercambiar por letras del alfabeto.
        istringstream indiceAlfabeto ( vectorBloques[ i ] );
        int valorNumerico;
        indiceAlfabeto >> valorNumerico;
        resultados.push_back( alfabeto[ valorNumerico ] );
    }

    resultados.push_back( ' ' );

    vectorBloques.clear();
    numeroBloques = Df.size() / hallarDigitos( ZZ( alfabeto.size() - 1 ) );//Crear bloques de 2 (cifra significativa alfabeto)
    vectorBloques = dividirBloques( NPublico, Df, numeroBloques, hallarDigitos( ZZ( alfabeto.size() - 1 ) ) );

    for( int i = 0; i < vectorBloques.size(); i++ ){//Eliminar espacios sobrantes.
        if( vectorBloques[ i ] == "" )
            vectorBloques[ i ] = "26";
    }
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Intercambiar por letras del alfabeto.
        istringstream indiceAlfabeto ( vectorBloques[ i ] );
        int valorNumerico;
        indiceAlfabeto >> valorNumerico;
        resultados.push_back( alfabeto[ valorNumerico ] );//Resultado en letras de la firma.
    }

    return resultados;

}
