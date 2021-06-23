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


RSA::RSA(){}
RSA::RSA(int bits){
    cout<<"Generando claves\n";
    p = generarPrimoNTL(bits);
    cout<<". ";
    cout<<endl<<p<<endl;
    q = generarPrimoNTL(bits);
    while(q == p){
        //cout<<"pam\n";
        q = generarPrimoNTL(bits);}
    cout<<". ";
    cout<<endl<<q<<endl;
    N = p*q;
    ZZ oN ( (p-1)*(q-1) );
    e = generarCoprimoNTL(oN, bits);
    cout<<".\n";
    d = mod(inversa_modularNTL(e,oN),oN);

    ostringstream salida;
    salida <<nombre<<"\t"<<e<<"\t"<<N<<"\t"<<correo<<endl;

    ofstream out("directorio_publico.txt");
        out<<salida.str()<<endl;
    out.close();

}

string RSA::cifrar(string &msg){
    cout<<"Ingrese la clave pública e: ";
    cin>>e;
    cout<<"\nIngrese N público: ";
    cin>>N;
    cout<<endl;
    unsigned int tamBloques = hallarDigitos(N)-1;

    int numCifras = hallarDigitos(ZZ(alfabeto.size()-1));

    string msg2;
    //Hallar números en el alfabeto.

    for(unsigned int i = 0; i<msg.size();i++){
        for(unsigned int j = 0; j < alfabeto.size();j++){
            if (alfabeto[j] == msg[i]){
                if(hallarDigitos(ZZ(j))<numCifras){
                    int diferenciaDigitos = numCifras - hallarDigitos(ZZ(j));
                    ostringstream digito;
                    for(int x = 0; x < diferenciaDigitos; x++)
                        digito << "0";
                    digito<<j;
                    msg2+=digito.str();
                }else{
                    ostringstream digito;
                    digito<<j;
                    msg2+=digito.str();
                }
            }
        }
    }

    //Dividir en bloques
    vector<string> vectorBloques;
    int numBloques;
    if(msg2.size()%tamBloques == 0){
        numBloques = msg2.size()/tamBloques;
    }else{
        numBloques = (msg2.size()/tamBloques)+1;
    }


    for(int i = 0; i < numBloques; i++){//Crea espacios vacíos.
        vectorBloques.push_back("");
    }

    int key = 0;
    for(unsigned int i = 0; i< msg2.size(); i++){//Rellenar los espacios
        vectorBloques[key].push_back(msg2[i]);
        if((i+1)%tamBloques == 0)key++;
    }

    if(vectorBloques.back().size() != tamBloques){//Comprobar si hay espacio sin rellenar.
        if((tamBloques-vectorBloques.back().size())%2 == 0){
            while(vectorBloques.back().size() < tamBloques){//Si la diferencia es par no es necesario un nuevo bloque
                vectorBloques.back()+="26";
            }
        }else{
            while(vectorBloques.back().size() < tamBloques-1){//Si la diferencia es impar agregar "26" y al final "2".
                vectorBloques.back()+="26";
            }
            vectorBloques.back().push_back('2');

            vectorBloques.push_back("6");
            while(vectorBloques.back().size() < tamBloques){//Rellenar nuevo bloque.
                vectorBloques.back()+="26";
            }
        }
    }


    //Cifrado
    for(int i = 0; i<vectorBloques.size();i++){
        istringstream valor(vectorBloques[i]);//COnvertir valores del vector a ZZ;
        ZZ valorConvertido;
        valor>>valorConvertido;


        ZZ valorCifrado( mod(Binary_Exponentiation(valorConvertido,e,N),N) );//Volver a convertir a string
        ostringstream nuevoValor;
        for(int j= 0; j < (hallarDigitos(N)-hallarDigitos(valorCifrado)); j++)
            nuevoValor<<"0";
        nuevoValor<<valorCifrado;

        vectorBloques[i] = nuevoValor.str();//Devolver al vector
    }

    //Convertir vector a string;
    string msgCifrado;
    for(int i = 0; i<vectorBloques.size();i++){
        msgCifrado += vectorBloques[i];
    }

    ofstream out("cifrado.txt");
        out<<msgCifrado<<endl;
    out.close();

    return msgCifrado;
}

string RSA::descifrar(string &msg){
    vector<string> vectorBloques;//Crear un vector para hacer más rápida la exponenciación modular.

    for(int i = 0; i < msg.size()/hallarDigitos(N); i++){//Crea espacios vacíos.
        vectorBloques.push_back("");
    }

    int key = 0;
    for(int i = 0; i< msg.size(); i++){//Rellenar los espacios
        vectorBloques[key].push_back(msg[i]);
        if((i+1)%hallarDigitos(N) == 0)key++;
    }



    for(int i = 0; i< vectorBloques.size(); i++){//Convertir a ZZ
        istringstream valores(vectorBloques[i]);
        ZZ valorConvertido;
        valores>>valorConvertido;
        //resto chino
        /*ZZ t1= mod(valorConvertido,p-1), t2= mod(valorConvertido,q-1);
        expon_euler a1(valorConvertido,t1);
        expon_euler a2(valorConvertido,t2);

        vector<ZZ> p_v{p,q};
        cout<<"primer vector check\n";
        vector<expon_euler> a{a1,a2};
        cout<<"segundo vector check\n";

        cout<<"valor: "<<valorConvertido<<endl;
        ZZ valorDescifrado ( resto_chino(a,p_v) );
        cout<<"nuevo valor: "<<valorDescifrado<<endl;*/

        //ZZ valorDescifrado ( mod(Binary_Exponentiation(valorConvertido,d,N),N) );//Exponenciacion modular
        cout<<"valor: "<<valorConvertido<<endl;
        ZZ valorDescifrado ( TRC(valorConvertido, d, p, q) );
        cout<<"nuevo valor: "<<valorDescifrado<<endl;

        ostringstream valoresNuevos;
         for(int j= 0; j < (hallarDigitos(N)-1-hallarDigitos(valorDescifrado)); j++)//Regresar a string añaidendo 0s a la izq
            valoresNuevos<<"0";
        valoresNuevos<<valorDescifrado;

        vectorBloques[i] = valoresNuevos.str();//Reemplazar en vector
    }

    key = 0;
    string msg2;
    for(int i = 0; i < vectorBloques.size();i++){//Pasar valores del vector a un string
        msg2+=vectorBloques[i];
    }
    vectorBloques.clear();//Limpiar vector
    int numeroCifras = hallarDigitos(ZZ( alfabeto.size()-1 ));

    for(int i = 0; i < (msg.size()/numeroCifras); i++){//Crea espacios vacíos.
        vectorBloques.push_back("");
    }
    for(int i = 0; i< msg2.size(); i++){//Rellenar los espacios
        vectorBloques[key].push_back(msg2[i]);
        if((i+1)%numeroCifras == 0)key++;
    }

    string resultado;
    for(int i = 0; i < vectorBloques.size();i++){
        istringstream indiceAlfabeto (vectorBloques[i]);
        int valorNumerico;
        indiceAlfabeto>>valorNumerico;
        resultado.push_back(alfabeto[valorNumerico]);
    }
    return resultado;
}
