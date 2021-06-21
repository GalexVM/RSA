#include <NTL/ZZ.h>
using NTL::ZZ;
using NTL::conv;
#include <string>
using std::string;
//Sin NTL
int moduloPositivo(long int dividendo, long int divisor);
int* euclidesExtendido(int r1, int r2, int s1=1, int s2=0, int t1=0, int t2=1);
int euclides(int r1, int r2);
bool esPrimo(int numero);
int generador_primos(int min, int max);
int generador_coprimos(int numero);
int inversa_modular(int n1, int n2);
int exponenciacionModular(int numero, int exponente, int modulo);

//Con NTL
ZZ mod(ZZ a, ZZ b);
ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n );
ZZ euclidesNTL(ZZ r1, ZZ r2);
vector <ZZ> euclides_ext(ZZ a, ZZ b);

ZZ middleSquareNumber(ZZ number, ZZ bits);
ZZ newTime();
string int_a_string(ZZ conversion);
ZZ string_a_int(string conversion);
ZZ pot(ZZ base, ZZ exponente);

bool MillerRabinTest(ZZ n, ZZ k =conv<ZZ>(10));
vector<ZZ>fact_2(ZZ n);
ZZ middleSquareNumberRan(ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor);

ZZ generarCoprimoNTL(ZZ n, int bits);
ZZ inversa_modularNTL(ZZ n1, ZZ n2);
ZZ generar_aleatorioNTL(int bits);
bool probarPrimoNTL(ZZ n);
ZZ generarPrimoNTL(int bits);
int hallarDigitos(ZZ n);
