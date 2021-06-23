#include <NTL/ZZ.h>
using NTL::ZZ;
using NTL::conv;
#include <string>
using std::string;


//Principales
ZZ mod(ZZ a, ZZ b);
ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n );//exponenciación.
ZZ resto_menor(ZZ a, ZZ b);//euclides.
vector <ZZ> euclides_ext(ZZ a, ZZ b);
ZZ middleSquareNumber(ZZ number, ZZ bits);
bool MillerRabinTest(ZZ n, ZZ k =conv<ZZ>(10));
ZZ inversa_modularNTL(ZZ n1, ZZ n2);
ZZ generarPrimoNTL(int bits);
ZZ generarCoprimoNTL(ZZ n, int bits);

//RestoChino
ZZ TRC(ZZ base, ZZ exponente, ZZ p, ZZ q);
class expon{
    public: ZZ base; int ex;
    expon(ZZ b){
       base= b; ex=0;
    }
    void sum(){
        ex++;
    }
};
class expon_euler{
    public: ZZ base; ZZ ex;
    expon_euler(ZZ b, ZZ e){
       base= b; ex=e;
    }
};
ZZ resto_chino(vector<expon_euler> a, vector<ZZ> p);
bool solucion( vector<ZZ> p);
ZZ ecuacion_mod_k0(ZZ a, ZZ b, ZZ c, ZZ n);
ZZ inversa_euler(ZZ base, ZZ m);
ZZ exponenciacion_euler(ZZ base, ZZ ex, ZZ m );
ZZ fi_euler(ZZ n);
ZZ prop4(expon n);
vector <expon> prop3(ZZ n );



//Auxiliares
ZZ newTime();//Obtener milisec
string int_a_string(ZZ conversion);
ZZ string_a_int(string conversion);
ZZ pot(ZZ base, ZZ exponente);
vector<ZZ>fact_2(ZZ n);//Sacar factores 2
ZZ middleSquareNumberRan(ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor);//Aleatorio en rango
int hallarDigitos(ZZ n);

