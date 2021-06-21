#pragma once
#include <string>
    using std::string;
#include <NTL/ZZ.h>
    using NTL::ZZ;


class RSA {
private:
    ZZ d;
    ZZ e;
    ZZ N;
    ZZ p;
    ZZ q;
    string alfabeto = "abcdefghijklmnopqrstuvwxyz ";//ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890
public:
    string nombre = "Gabriel Valdivia Medina";
    string correo = "gabriel.valdivia.medina@ucsp.edu.pe";
    RSA ();
    RSA(int bits);
    string cifrar(string &msg);
    string descifrar(string &msg);

};



