#pragma once
#include <string>
    using std::string;
#include <NTL/ZZ.h>
    using NTL::ZZ;
#include <vector>
    using std::vector;

class RSA {
private:
    ZZ d;
    ZZ e;
    ZZ N;
    ZZ p;
    ZZ q;
    string firma;
    string alfabeto = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
public:
    string nombre = "Gabriel Valdivia Medina";
    string correo = "gabriel.valdivia.medina@ucsp.edu.pe";
    RSA( int bits, int a );
    RSA( int bits );
    string descifrar( string &msg );
    string cifrar( string &msg );
};



