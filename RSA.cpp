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

    ostringstream salida;
    salida <<e<<"\t"<<N<<"\t"<<endl;//Crear cadena con datos.
    ofstream out("claves_emisor.txt");//Imprimir en txt
        out<<salida.str()<<endl;
    out.close();
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
    salida <<e<<"\t"<<N<<"\t"<<endl;//Crear cadena con datos.
    ofstream out("claves_receptor.txt");//Imprimir en txt
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

    //C
    string msgConvertido =  alfabetoANumeros ( msg, alfabeto, NPublico );
    vector < string > vectorBloques ( separarBloques( msgConvertido, NPublico ) );

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
    string firmaConvertida =  alfabetoANumeros ( firma, alfabeto, N );
    vectorBloques = separarBloques( firmaConvertida, N );
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
    cout<<"firma cifrada: "<<firmaCifrada<<endl;
    //Cr
    vectorBloques = separarBloques( firmaCifrada, NPublico );
    //vectorBloques = dividirBloques( NPublico, firmaCifrada, firmaCifrada.size(), hallarDigitos( NPublico ) - 1 );
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
    cout<<"firma final"<<firmaFinal<<endl;
    ofstream out( "cifrado.txt" );//Imprimir en txt.
        out << msgCifrado << endl << firmaFinal << endl;
    out.close();

    string resultado;
    resultado = msgCifrado + " " + firmaFinal;
    //vector < string > resultado = { msgCifrado, firmaFinal };
    return resultado;
}

string RSA::descifrar( string &msg ) {
    string firm;
    string msg0;
    bool cambioClave = false;
    for( unsigned int i = 0; i < msg.size(); i++ ){
        if( cambioClave == true ){
            firm.push_back( msg[ i ] );
        }else{
            if( msg[ i ] != ' ')
                msg0.push_back( msg[ i ] );
        }
        if( msg[ i ] == ' ' )
            cambioClave = true;
    }

    cout<<"msg: "<<'|'<<msg0<<'|'<<endl;
    cout<<"firm: "<<firm<<endl;

    ZZ ePublico;
    ZZ NPublico;
    cout << "Ingrese la clave pública e de su remitente: ";
    cin >> ePublico;
    cout<<"e ingresado\n";
    cout << "\nIngrese la clave pública N de su remitente: ";
    cin >> NPublico;
    cout<<"N ingresado\n";
    cout << endl;
    //D
    vector< string > vectorBloques = dividirBloques( N, msg0, msg0.size() / hallarDigitos( N ), hallarDigitos( N ) );
    for( int i = 0; i < vectorBloques.size(); i++ )
        cout << vectorBloques[ i ] << ',';
    cout<<endl;

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
    cout<<"termino D\n";
    cout<<msg2<<endl;
    //Dr
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
    cout<<"termino Dr\n";
    cout<<Dr<<endl;
    //Df
    vector< string > vectorBloques3 = dividirBloques( NPublico, Dr, Dr.size() / hallarDigitos( NPublico ), hallarDigitos( NPublico ) );
    cout << "dividio bloques\n";
    for( int i = 0; i < vectorBloques3.size(); i++ ){//Bloque por bloque...
        ZZ valorConvertido ( string_a_ZZ( vectorBloques3[ i ] ) );
        //ZZ valorDescifrado ( TRC( valorConvertido, d, p, q ) );//Teorema del Resto Chino
        ZZ valorDescifrado ( Binary_Exponentiation( valorConvertido, ePublico, NPublico ) );
        cout << "exponencio el valor "<<i<<endl;
        ostringstream valoresNuevos;
         for( int j = 0; j < ( hallarDigitos( NPublico ) - 1 - hallarDigitos( valorDescifrado ) ); j++ )//Regresar a string añaidendo 0s a la izq
            valoresNuevos << "0";
        valoresNuevos << valorDescifrado;
        vectorBloques3[ i ] = valoresNuevos.str();//Reemplazar en vector
    }
    string Df;
    for( int i = 0; i < vectorBloques3.size(); i++ )//Resultado numérico.
        Df += vectorBloques3[ i ];
    cout<<"termino Df\n";
    cout<<Df<<endl;
    //Convertir a letras.
    vectorBloques.clear();
    int numeroBloques = msg0.size() / hallarDigitos( ZZ( alfabeto.size() - 1 ) );//Crear bloques de 2 (cifra significativa alfabeto)
    vectorBloques = dividirBloques( N, msg2, numeroBloques, hallarDigitos( ZZ( alfabeto.size() - 1 ) ) );

    for( int i = 0; i < vectorBloques.size(); i++ ){//Eliminar espacios sobrantes.
        if( vectorBloques[ i ] == "" )
            vectorBloques[ i ] = "26";
    }

    string resultados;//Resultado en letras D.
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Intercambiar por letras del alfabeto.
        istringstream indiceAlfabeto ( vectorBloques[ i ] );
        int valorNumerico;
        indiceAlfabeto >> valorNumerico;
        resultados.push_back( alfabeto[ valorNumerico ] );
    }
    cout<<"convirio D a letras\n";
    resultados.push_back( ' \n ' );

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
        resultados.push_back( alfabeto[ valorNumerico ] );
    }
    cout<<"convirio Df a letras\n";
    return resultados;
}
