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
    firma = "Gabriel Alexander Valdivia Medina 201 10 47904 73127083 Dayevska Anabel Caceres Budiel 201 10 47457 73333523 Sergio Leandro Ramos Villena  201 10 47800 75974804 Roberto Juan Cayro Cuadros 201 10 47988 71508951 ";
    p = generarPrimoNTL( bits );
    //p = conv<ZZ>("162431632190270390844677294508066935760423654000418860836210464348717462624150304123359289955951601345792667992960098029917148285205114113819018020185713768498268359894234118073157381865778781837488205594067672696265203337438770759090488264252320694611645437877355284557303736617236980381913600275478682463583");

        //cout<<"p emisor: "<<p<<endl;
        cout<<". ";
    q = generarPrimoNTL( bits );
    //q = conv<ZZ>("107995336702114952360433889360185652212818175232735054412104714787821776522389306800014210133138526794417935345859900147677521385492281896489010685465248719696209248546390486412916281351579715015502034524065282637231159795265567651535646561263358904663320482749055446569944058757214556368808199223575042822037");


        cout<<". ";
    while( q == p )
        q = generarPrimoNTL( bits );
    //cout<<"q emisor: "<<q<<endl;
    N = p * q;
    //cout<<"N emisor: "<<N<<endl;
    ZZ oN ( ( p - 1 ) * ( q - 1 ) );
    e = generarCoprimoNTL( oN, bits );
    //e = conv<ZZ>("175247984702926922117780212760712796707900834313894915890777476416493332514174895010401858921980633690040910400491637090393906000424019915432106615787104901635185858294945905409903991650104381338475494319884276986494862393524545090698909186890925781899277819115796582684728008933004124648989265593574009073765");

        //cout<<"e emisor: "<<e<<endl;
        cout<<".\n";
    d = mod( inversa_modularNTL( e, oN ),oN );
        //cout<<"d emisor: "<<d<<endl;

    ostringstream salida;//Enviar claves al txt "claves_emisor"
    salida <<e<<"\t"<<N<<"\t"<<endl;
    ofstream out("claves_emisor.txt");
        out<<salida.str()<<endl;
    out.close();
}
RSA::RSA( int bits, int a ) {
    cout << "Generando claves\n";
    p = generarPrimoNTL( bits );
    //p = conv<ZZ>("139499834813316884953155041352669588224038448715298830438931567603861414637507234643328828327518841915342418452724753256589392960513156471915276775646601857450654804807565646326997770720924609758645701297951123899867938894835565587017575327486313351965524090394084545251042649798413357449072316941464296290061");

        //cout<<"p receptor: "<<p<<endl;
        cout<<". ";
    //q = conv<ZZ>("124223839954149416303430873356210262945656163614237355189701105408755156465900986367416302171545875240084758465163141485206694527172876264401809022910865266412496715559372057515405510858994283537095110104081871519011931163649594237428452580940122778024942288684665722454461078347737268405065212885190528448739");
    q = generarPrimoNTL( bits );
    while( q == p ){
        q = generarPrimoNTL( bits );}
        //cout<<"q receptor: "<<q<<endl;
        cout << ". ";
    N = p * q;
        //cout<<"N receptor: "<<N<<endl;
    ZZ oN ( ( p - 1 ) * ( q - 1 ) );
    e = generarCoprimoNTL( oN, bits );
    //e = conv<ZZ>("114249074242817760683473488436589463507341717625756008908553333630094852941819799461955892708833266780908926976338500242415976270555761013532173357386850057561574589440017644017607554843327517106679679630683134291305165841889295926141735386351536969742644948977484171682840300527929388840835894526430783375011");
        //cout<<"e receptor: "<<e<<endl;
        cout<<".\n";
    d = mod( inversa_modularNTL( e, oN ),oN );
        //cout<<"d receptor: "<<d<<endl;

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
            vectorBloques[ i ] = "30";
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
            vectorBloques[ i ] = "30";
    }
    for( int i = 0; i < vectorBloques.size(); i++ ) {//Intercambiar por letras del alfabeto.
        istringstream indiceAlfabeto ( vectorBloques[ i ] );
        int valorNumerico;
        indiceAlfabeto >> valorNumerico;
        resultados.push_back( alfabeto[ valorNumerico ] );//Resultado en letras de la firma.
    }

    return resultados;

}
