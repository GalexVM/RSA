#include <iostream>
using std::cout;
using std::endl;
#include <NTL/ZZ.h>
using NTL::ZZ;
using NTL::conv;
using NTL::RandomBits_ZZ;
#include <ctime>
#include <cstdlib>
#include <vector>
using std::vector;
#include "math-functions.h"
#include <sys/time.h>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
#include <sstream>
using std::ostringstream;
using std::istringstream;



//NTL

ZZ mod(ZZ a, ZZ b){
	ZZ q= a/b;
	ZZ	r= a- (q*b);
	if(a<ZZ(0)){
		ZZ ar=r;
	    r= b+ar;
	}
	return r;
}

ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n ) {
    ZZ result (1);
    while( e != ZZ(0)) {
        if( mod(e,ZZ(2)) == ZZ(1))
            result  = mod(result*a,n);
        e >>= 1;
        a = mod(a*a,n);
    }
    return result;
}
//Menor resto
ZZ min_(ZZ a, ZZ b){
    ZZ c;

    if (a <= b){
        c=a;
}
    else if (a>b){
        c=b;
    }
return c;
}

ZZ rem(ZZ c, ZZ d){

    ZZ r1=((d*((c/d)+1))-c);
    ZZ r2=c-(d*(c/d));

    ZZ r3=min_(r1,r2);
    return r3;
}

ZZ resto_menor(ZZ a, ZZ b){
    ZZ c,d,r;
    if (a==0){
        c=b;}
    else{
            c=a;
            d=b;
        while(d!=0){
            r=rem(c,d);
            c=d;
            d=r;}
    }
    return c;
}
//
ZZ euclidesNTL(ZZ r1, ZZ r2){
	ZZ q,r;
	while(r2>0){
		q = r1/r2;
		r = r1 - (q*r2);
		r1 = r2;
		r2 = r;
	}
	return r1;
}
ZZ d(0),k(0),l(0);
vector <ZZ>  euclides_ext(ZZ a, ZZ b){

  vector<ZZ> res = {d,k, l};
  if (b==ZZ(0))  return res = {a,ZZ(1),ZZ(0)}; //caso base

  ZZ q = mod(a,b);
  ZZ r = (a-q)/b;

  res = euclides_ext(b, q);
  d=res[0];
  k=res[1];
  l=res[2];
  return res = {d,l,k-l*r};
}

ZZ inversa_modularNTL(ZZ n1, ZZ n2){
	ZZ res = euclides_ext(n1,n2)[1];
	return res;
}

//middleSquare
ZZ middleSquareNumber(ZZ number, ZZ bits) {
   ZZ intervalo_mayor = pot(ZZ(2),bits)-1;
   ZZ intervalo_menor = pot(ZZ(2),bits)/2;

   ZZ sqn = number * number, next_number = number;
   while(next_number < intervalo_mayor)
   {
     int tam0  = int_a_string(next_number).length();
     ZZ tam1 = conv<ZZ>(tam0);
     ZZ t (tam1 / ZZ(2));
     ZZ limite = pot(ZZ(10),t);
      sqn =  next_number / limite;
      next_number = sqn * sqn * sqn;

   }
   next_number = mod(next_number, intervalo_mayor);
   if(next_number < intervalo_menor)
   {
     next_number += intervalo_menor;
   }
   return next_number;
}
ZZ newTime()
{
    ZZ x;
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    x = ZZ(millisec_since_epoch);

    x = string_a_int(int_a_string(x).substr(6,9));

    x = pot(x+5,ZZ(3));
    return x;
}

string int_a_string(ZZ conversion){
  ostringstream convertido;
  convertido << conversion;
  return convertido.str();
}

ZZ string_a_int(string conversion){
  istringstream convertido(conversion);
  ZZ entero;
  convertido >> entero;
  return entero;
}

ZZ pot(ZZ base, ZZ exponente) {
    if (exponente == 0) return ZZ(1);
    ZZ x = pot(base, exponente/2);
    if (mod(exponente,ZZ(2))==0) return x*x;
    return x*x*base;
}
//
//---------------------------------------------------------------------------------------------------------------------------------
//MillerRabin
bool MillerRabinTest(ZZ n, ZZ k){
   bool continueFor=false;
    //1. Write n as 2^r*d + 1; d odd; factoring out of n-1.
    ZZ r(0);
    ZZ d(1);
    vector<ZZ>facts = fact_2(ZZ(n-1));
    r = facts[0];
    d = facts[1];


    //2. WitnessLoop.
    ZZ a,x;

    for(int i = 0; i < k; i++){//Repeat k times

        ZZ aux (newTime());
        a = middleSquareNumberRan(aux,n-2,ZZ(2));
        x = Binary_Exponentiation(a,d,n);

        if(x == 1 || x == n - 1)
            continue;
        for(int i = 0; i < r-1; i++){//repeat r-1 times
            x = mod(x*x,n);
            if(x == n-1){
                continueFor=true;
                break;
            }
        }
        if(continueFor==true)continue;
        return false;
    }
    return true;
}

vector<ZZ>fact_2(ZZ n){
    vector<ZZ>facts;
    ZZ a (0);
    while(mod(n,ZZ(2))==0){
        n = n/2;
        a++;
    }
    facts.push_back(a);
    facts.push_back(n);
    return facts;
}
ZZ middleSquareNumberRan(ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor) {

   ZZ sqn = number * number, next_number = number;
   while(next_number < intervalo_mayor)
   {
     int tam0  = int_a_string(next_number).length();
     ZZ tam1 = conv<ZZ>(tam0);
     ZZ t (tam1 / ZZ(2));
     ZZ limite = pot(ZZ(10),t);
     sqn =  next_number / limite;
     next_number = sqn * sqn * sqn;

   }
   next_number = mod(next_number, intervalo_mayor);
   if(next_number < intervalo_menor)
   {
     next_number += intervalo_menor;
   }

   return next_number;
}
//
ZZ generarPrimoNTL(int bits){
    ZZ numero (middleSquareNumber(newTime(),ZZ(bits)));
    while (mod(numero,ZZ(2))==0 || MillerRabinTest(numero) == false){
        numero = middleSquareNumber(newTime(),ZZ(bits));
    }
    return numero;
}


ZZ generarCoprimoNTL(ZZ n, int bits){
    ZZ coprimo ( middleSquareNumber( newTime(),ZZ( bits ) ) );
    if(coprimo > n){
        while(resto_menor(coprimo,n)!=1)
            coprimo = middleSquareNumber( newTime(),ZZ( bits ) ) ;
    }else{
        while(resto_menor(n,coprimo)!=1)
            coprimo = middleSquareNumber( newTime(),ZZ( bits ) ) ;
    }
    return coprimo;
}


int hallarDigitos(ZZ n){
    if(n == 0) return 1;
    int counter = 0;
    while(n >=1){
        counter+=1;
        n = n/10;
    }
    return counter;
}
