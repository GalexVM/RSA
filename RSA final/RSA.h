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
    string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.-( )abcdefghijklmnopqrstuvwxyz<>*1234567890[]";
public:
    RSA( int bits, int a );
    RSA( int bits );
    string descifrar( string &msg );
    string cifrar( string &msg );
};



