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
#include <bitset>
#include <time.h>

//Principales----------------------------------------------------------------------------------------------------------------------------------

ZZ mod( ZZ a, ZZ b ){
	ZZ q = a / b;
	ZZ	r = a- ( q * b );
	if( a < ZZ( 0 ) ) {
		ZZ ar = r;
	    r= b + ar;
	}
	return r;
}

ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n ) {
    ZZ result ( 1 );
    while( e != ZZ( 0 ) ) {
        if( mod( e, ZZ( 2 ) ) == ZZ( 1 ) )
            result  = mod( result * a, n );
        e >>= 1;
        a = mod( a * a, n );
    }
    return result;
}

ZZ resto_menor( ZZ a, ZZ b ){
    ZZ c, d, r;
    if ( a == 0 )
        c = b;
    else{
        c = a;
        d = b;
        while( d != 0 ){
            r = rem( c, d );
            c = d;
            d = r;
        }
    }
    return c;
}

ZZ d( 0 ), k( 0 ), l( 0 );
vector <ZZ>  euclides_ext( ZZ a, ZZ b ){
  vector<ZZ> res = { d, k, l };
  if ( b == ZZ( 0 ) ) return res = { a, ZZ( 1 ), ZZ( 0 ) }; //caso base
  ZZ q = mod( a, b );
  ZZ r = ( a - q ) / b;
  res = euclides_ext( b, q );
  d = res[ 0 ];
  k = res[ 1 ];
  l = res[ 2 ];
  return res = { d, l, k-l * r };
}

ZZ inversa_modularNTL( ZZ n1, ZZ n2 ){
	ZZ res = euclides_ext( n1, n2 )[ 1 ];
	return res;
}

ZZ middleSquareNumber( ZZ number, ZZ bits ) {//N�meros aleatorios seg�n bits.
   ZZ intervalo_mayor = pot( ZZ( 2 ), bits ) - 1;
   ZZ intervalo_menor = pot( ZZ( 2 ), bits) / 2;
   ZZ sqn = number * number, next_number = number;

   while( next_number < intervalo_mayor ){
        int tam0 = ZZ_a_string( next_number ).length();
        ZZ tam1 = conv<ZZ>( tam0 );
        ZZ t ( tam1 / ZZ( 2 ) );
        ZZ limite = pot( ZZ( 10 ), t );
        sqn =  next_number / limite;
        next_number = sqn * sqn * sqn;
   }

   next_number = mod( next_number, intervalo_mayor );
   if ( next_number < intervalo_menor ) {
     next_number += intervalo_menor;
   }
   return next_number;
}

bool MillerRabinTest( ZZ n, ZZ k ) {//Comprobador de primalidad probabil�stico (10 tests).
   bool continueFor = false;
    //1. Write n as 2^r*d + 1; d odd; factoring out of n-1.
    ZZ r( 0 );
    ZZ d( 1 );
    vector <ZZ> facts = fact_2( ZZ( n - 1 ) );
    r = facts[ 0 ];
    d = facts[ 1 ];
    //2. WitnessLoop.
    ZZ a, x;
    for( int i = 0; i < k; i++ ) {//Repeat k times
        ZZ aux ( newTime() );
        a = middleSquareNumberRan( aux, n - 2, ZZ( 2 ) );
        x = Binary_Exponentiation( a, d, n );
        if( x == 1 || x == n - 1 )
            continue;
        for( int i = 0; i < r - 1; i++) {//repeat r-1 times
            x = mod( x * x, n );
            if(x == n - 1 ) {
                continueFor = true;
                break;
            }
        }
        if( continueFor == true ) continue;
        return false;
    }
    return true;
}

ZZ generarPrimoNTL( int bits ) {
    auto begin = std::chrono::high_resolution_clock::now();
    ZZ numero ( middleSquareNumber( newTime(), ZZ( bits ) ) );
    if( bits <= 16 ){
        srand( time( nullptr ) );
        ZZ random;
        if( bits == 4 ) {
            random = middleSquareNumberRan(newTime(),ZZ(2),ZZ(1));
            numero = criba_eratostenes( bits, random );
        }
        else {
            random = middleSquareNumberRan(newTime(),ZZ(2),ZZ(1));
            numero = criba_eratostenes( bits, random );
        }
    }
    while ( mod( numero, ZZ( 2 ) ) == 0 || MillerRabinTest( numero ) == false ) {
        numero = middleSquareNumber( newTime(), ZZ( bits ) );
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast <std::chrono::nanoseconds> ( end - begin );
        int seg = elapsed.count()*1e-9;
        //cout<<seg<<endl;
        if( seg > 30 && bits >= 2048) {
            cout << "\nTiempo de espera agotado, generando n�meros con m�todo alternativo... \n" << seg << endl;
            numero = criba_eratostenes( bits, ZZ( 1 ) );
            break;
        }
    }
    return numero;
}

ZZ generarCoprimoNTL( ZZ n, int bits ) {
    ZZ coprimo ( middleSquareNumber( newTime(),ZZ( bits ) ) );
    if( coprimo > n ) {
        while( resto_menor( coprimo, n ) != 1 )
            coprimo = middleSquareNumber( newTime(),ZZ( bits ) ) ;
    } else {
        while( resto_menor( n, coprimo ) != 1 )
            coprimo = middleSquareNumber( newTime(),ZZ( bits ) ) ;
    }
    return coprimo;
}

ZZ criba_eratostenes( int bits, ZZ limite ) {
    ZZ pot = NTL::power2_ZZ( bits );
    ZZ max = pot - 1;
    nodo *h = new nodo ( ZZ( 0 ), ZZ( 0 ) );
    nodo *t = h;
    if( limite != ZZ( 0 ) ) {
        int contador = 0;
        for ( ZZ min = pot / 2; ZZ( contador ) < limite; min++ ) {
            if ( MillerRabinTest( conv <ZZ> ( min ), ZZ( 10 ) ) ) {
                contador++;
                t -> next = new nodo( ( t -> key ) + 1, min );
                t = t -> next;
            }
        }
        ZZ valor = t -> val;
        return valor;
    }
    else {
        for ( ZZ min = pot / 2; min <= max; min++ ) {
            if ( MillerRabinTest( conv <ZZ>( min ),ZZ( 10 ) ) ) {
                t -> next = new nodo( ( t -> key ) + 1, min );
                t= t -> next;
            }
        }
        ZZ valor = t -> val;
        return valor;
    }
}

ZZ TRC( ZZ base, ZZ exponente, ZZ p, ZZ q ) {//Teorema del resto chino
    ZZ P ( p * q );
    ZZ P1 ( q );
    ZZ P2 ( p );
    ZZ q1 ( inversa_modularNTL( P1, p ) );
    ZZ q2 ( inversa_modularNTL( P2, q ) );
    ZZ exponente1 ( mod( exponente,p - 1 ) );
    ZZ exponente2 ( mod( exponente,q - 1 ) );
    ZZ X ( mod( ( Binary_Exponentiation( base, exponente1, p ) * P1 * q1 ) + ( Binary_Exponentiation( base, exponente2, q ) * P2 * q2 ), P ) );
    X = mod( X, P );
    return X;
}

//Auxiliares--------------------------------------------------------------------------------------------------------------------------------------
ZZ min_( ZZ a, ZZ b ) {//Ayuda al menor resto.
    ZZ c;
    if ( a <= b ) {
        c = a;
    }
    else if ( a > b ) {
        c = b;
    }
return c;
}

ZZ rem( ZZ c, ZZ d ) {//Ayuda al menor resto.
    ZZ r1 = ( ( d * ( ( c / d ) + 1 ) ) - c );
    ZZ r2 = c - ( d * ( c / d ) );
    ZZ r3 = min_( r1, r2 );
    return r3;
}

ZZ newTime(){
    ZZ x;
    auto millisec_since_epoch = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
    x = ZZ( millisec_since_epoch );
    x = string_a_ZZ( ZZ_a_string( x ).substr( 6, 9 ) );
    if( x == 0 || x == 10 || x == 100 || x == 1000 ) {
        x = newTime();
    }
    x = pot( x + 5, ZZ( 3 ) );
    return x;
}

string ZZ_a_string( ZZ conversion ) {
  ostringstream convertido;
  convertido << conversion;
  return convertido.str();
}

ZZ string_a_ZZ( string conversion ) {
  istringstream convertido( conversion );
  ZZ entero;
  convertido >> entero;
  return entero;
}

ZZ pot( ZZ base, ZZ exponente ) {
    if ( exponente == 0 ) return ZZ( 1 );
    ZZ x = pot( base, exponente / 2 );
    if ( mod( exponente, ZZ( 2 ) ) == 0 ) return x * x;
    return x * x * base;
}

vector< ZZ >fact_2( ZZ n ) {
    vector< ZZ > facts;
    ZZ a ( 0 );
    while( mod( n, ZZ( 2 ) ) == 0 ) {
        n = n / 2;
        a++;
    }
    facts.push_back( a );
    facts.push_back( n );
    return facts;
}
ZZ middleSquareNumberRan( ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor) {
   ZZ sqn = number * number, next_number = number;
   while( next_number < intervalo_mayor ) {
     int tam0  = ZZ_a_string( next_number ).length();
     ZZ tam1 = conv< ZZ >( tam0 );
     ZZ t ( tam1 / ZZ( 2 ) );
     ZZ limite = pot( ZZ( 10 ), t );
     sqn =  next_number / limite;
     next_number = sqn * sqn * sqn;
   }
   next_number = mod( next_number, intervalo_mayor );
   if( next_number < intervalo_menor ) {
     next_number += intervalo_menor;
   }
   return next_number;
}

int hallarDigitos( ZZ n ) {
    if( n == 0 ) return 1;
    int counter = 0;
    while( n >= 1 ) {
        counter += 1;
        n = n / 10;
    }
    return counter;
}
