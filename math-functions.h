#include <NTL/ZZ.h>
using NTL::ZZ;
using NTL::conv;
#include <string>
using std::string;

//Principales
ZZ mod( ZZ a, ZZ b );
ZZ Binary_Exponentiation( ZZ a, ZZ e, ZZ n );//exponenciación.
ZZ resto_menor( ZZ a, ZZ b );//euclides.
vector <ZZ> euclides_ext( ZZ a, ZZ b );
ZZ middleSquareNumber( ZZ number, ZZ bits );
bool MillerRabinTest( ZZ n, ZZ k = conv< ZZ >( 10 ) );
ZZ inversa_modularNTL( ZZ n1, ZZ n2 );
ZZ generarPrimoNTL( int bits );
ZZ generarCoprimoNTL( ZZ n, int bits );
ZZ TRC( ZZ base, ZZ exponente, ZZ p, ZZ q );

//Auxiliares
ZZ newTime();//Obtener milisec
string ZZ_a_string( ZZ conversion );
ZZ string_a_ZZ( string conversion );
ZZ pot( ZZ base, ZZ exponente );
vector< ZZ >fact_2( ZZ n );//Sacar factores 2
ZZ middleSquareNumberRan( ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor );//Aleatorio en rango
int hallarDigitos( ZZ n );
ZZ min_( ZZ a, ZZ b );
ZZ rem( ZZ c, ZZ d );
string alfabetoANumeros ( string msg, string &alfabeto, ZZ N );
vector< string > separarBloques( string &msg, int Size, ZZ N );
vector < string > dividirBloques ( ZZ &N, string &msg, int numBloques, int tamBloques );

//Criba
ZZ criba_eratostenes( int bits, ZZ limite );
class nodo {
public:
    ZZ key, val ;
    nodo *next ;
    nodo ( ZZ k, ZZ v, nodo *n = nullptr ) : key ( k ), val( v ), next ( n ) {}

    bool find_key ( nodo* head, ZZ k, nodo *& pos ){
        pos = nullptr;
        nodo *t = head -> next;
        for ( ; t and t -> key < k ;pos = t, t = t -> next );
        if( t and t -> key == k ) return 1;
        else return 0;
    }
    void add( nodo* head, ZZ v ) {
        nodo *pos = nullptr;
        nodo *t = head -> next;
        for ( ; t; pos = t, t = t -> next );
        pos -> next = new nodo( ( pos -> key ) + 1, v);
    }
    ~nodo(){
        cout << "delete" << endl;
    }
};
