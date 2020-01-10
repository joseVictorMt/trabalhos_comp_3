#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>

#include "apply.cc"

using namespace std;

double seno( double n ) { return sin(n); }
int id( int n ) { return n; }
string roman( int n ) {
    map<int,string> rom = { { 1, "I" }, { 2, "II" }, { 3, "III" }, { 4, "IV" }, { 5, "V" }, { 6, "VI" }, { 7, "VII" }, { 8, "VIII" } } ;

    return rom[n];
}

struct FunctorSimples {
    string operator()( int n ) { return roman( n ) + "!"; }
};

struct FunctorTemplate {
    template <typename T>
    T operator()( T n ) { return n+n; }
};

template <typename T>
ostream& operator << ( ostream& o, const vector<T>& v ) {
    o << "[ ";
    for( auto x : v )
        o << x << " ";

    return o << "]";
}

int main( int argc, char* argv[]) {
    // Teste 1
    vector<int> v{ 1, 2, 3, 4, 5 };
    vector<int> r = apply( v, id );
    cout << r << endl;

    // Teste 2
    vector<int> v2{ 1, 2, 3, 4, 5 };
    vector<double> r2 = apply( v2, seno );
    cout << r2;

    cout << endl;

    //Teste 3
    auto v3 = { 1, 2, 3, 4, 5 };
    cout << apply( v3, []( auto n ) -> double { return n*n + 0.1; } );
    cout << endl;

    //Teste 5
    // auto a = { 1, 2 }, b = { 3, 4 };
    // cout << apply( a, [b]( auto x ) { return apply( b, [x]( auto y ){ return vector{ x, y }; } ); } );
    // cout << endl;

    //Teste 8
    cout << apply( { 1, 2, 3, 4, 5 }, roman );

    cout << endl;

    // Teste 9
    int v9[] = { 1, 2, 3, 4, 5, 6, 7 };
    vector<int> r9 = apply( v9, id );
    cout << r9 << endl;
}
