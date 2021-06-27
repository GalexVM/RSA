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
    firma = "Gabriel Alexander Valdivia Medina 201 10 47904 73127083 Dayevska Anabel Caceres Budiel 201 10 47457 73333523 Sergio Leandro Ramos Villena  201-10-47800 75974804 Roberto Juan Cayro Cuadros 201 10 47988 71508951";
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
}
RSA::RSA( int bits, int a ) {
    cout << "Generando claves\n";
    p = generarPrimoNTL( bits );
    cout<<". ";
   // cout<<endl<<p<<endl;
    q = generarPrimoNTL( bits );
    while( q == p ){
        //cout<<"pam\n";
        q = generarPrimoNTL( bits );}
    cout << ". ";
    //cout<<endl<<q<<endl;
    N = p * q;
    ZZ oN ( ( p - 1 ) * ( q - 1 ) );
    e = generarCoprimoNTL( oN, bits );
    cout<<".\n";
    d = mod( inversa_modularNTL( e, oN ),oN );

    ostringstream salida;
    salida <<nombre<<"\t"<<e<<"\t"<<N<<"\t"<<correo<<endl;//Crear cadena con datos.
    ofstream out("directorio_publico.txt");//Imprimir en txt
        out<<salida.str()<<endl;
    out.close();
}

 string  RSA::cifrar( string &msg ) {
    ZZ ePublico;
    ZZ NPublico;
    cout << "Ingrese la clave pública e: ";
    cin >> ePublico;
    cout << "\nIngrese N público: ";
    cin >> NPublico;
    cout << endl;

    //C
    vector < string > vectorBloques ( separarBloques( msg, alfabeto, NPublico ) );
    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, ePublico, NPublico ), NPublico ) );//Volver a convertir a string

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( NPublico ) - hallarDigitos( valorCifrado ) ); j++ )
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string msgCifrado;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        msgCifrado += vectorBloques[ i ];
    }
    //r
    vectorBloques = separarBloques( firma, alfabeto, N );
    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, d, N ), N ) );//Volver a convertir a string

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( NPublico ) - hallarDigitos( valorCifrado ) ); j++ )
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string firmaCifrada;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        firmaCifrada += vectorBloques[ i ];
    }
    //Cr
    vectorBloques = separarBloques( firmaCifrada, alfabeto, NPublico );
    for( int i = 0; i < vectorBloques.size(); i++ ) {
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );

        ZZ valorCifrado( mod( Binary_Exponentiation( valorConvertido, ePublico, NPublico ), NPublico ) );//Volver a convertir a string

        ostringstream nuevoValor;
        for( int j = 0; j < ( hallarDigitos( NPublico ) - hallarDigitos( valorCifrado ) ); j++ )
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[ i ] = nuevoValor.str();//Devolver al vector
    }

    string firmaFinal;
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Convertir vector a string;
        firmaFinal += vectorBloques[ i ];
    }
    ofstream out( "cifrado.txt" );//Imprimir en txt.
        out << msgCifrado << endl << firmaFinal << endl;
    out.close();

    string resultado;
    resultado = msgCifrado + " " + firmaFinal;
    //vector < string > resultado = { msgCifrado, firmaFinal };
    return resultado;
}

string RSA::descifrar( string &msg ) {
    vector< string > vectorBloques;//Crear un vector para hacer más rápida la exponenciación modular.
    for( int i = 0; i < msg.size() / hallarDigitos( N ); i++ ) {//Crea espacios vacíos.
        vectorBloques.push_back("");
    }
    int key = 0;
    for( int i = 0; i < msg.size(); i++ ) {//Rellenar los espacios
        vectorBloques[ key ].push_back( msg[ i ] );
        if( mod( ZZ( i + 1 ), ZZ( hallarDigitos( N ) ) ) == 0 ) key++;
    }

    for( int i = 0; i < vectorBloques.size(); i++ ){//Convertir a ZZ
        ZZ valorConvertido ( string_a_ZZ( vectorBloques[ i ] ) );
        ZZ valorDescifrado ( TRC( valorConvertido, d, p, q ) );//Teorema del Resto Chino

        ostringstream valoresNuevos;
         for( int j = 0; j < ( hallarDigitos( N ) - 1 - hallarDigitos( valorDescifrado ) ); j++ )//Regresar a string añaidendo 0s a la izq
            valoresNuevos << "0";
        valoresNuevos << valorDescifrado;
        vectorBloques[ i ] = valoresNuevos.str();//Reemplazar en vector
    }

    key = 0;
    string msg2;
    for( int i = 0; i < vectorBloques.size(); i++ ){//Pasar valores del vector a un string
        msg2 += vectorBloques[ i ];
    }
    vectorBloques.clear();//Limpiar vector
    int numeroCifras = hallarDigitos( ZZ( alfabeto.size() - 1 ) );
    for( int i = 0; i < ( msg.size() / numeroCifras ); i++ ) {//Crea espacios vacíos.
        vectorBloques.push_back("");
    }
    for( int i = 0; i < msg2.size(); i++ ) {//Rellenar los espacios
        vectorBloques[ key ].push_back( msg2[ i ] );
        if( mod( ZZ( i + 1 ), ZZ( numeroCifras ) ) == 0) key++;
    }
    for( int i = 0; i < vectorBloques.size(); i++ ){//Eliminar espacios sobrantes.
        if( vectorBloques[ i ] == "" )
            vectorBloques[ i ] = "26";
    }
    string resultado;
    for( int i = 0; i < vectorBloques.size(); i++ ) {
        istringstream indiceAlfabeto ( vectorBloques[ i ] );
        int valorNumerico;
        indiceAlfabeto >> valorNumerico;
        resultado.push_back( alfabeto[ valorNumerico ] );
    }
    return resultado;
}
