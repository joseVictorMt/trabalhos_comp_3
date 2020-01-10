#include <iostream>
#include <algorithm>
#include <array>

#include "bind.cc"

using namespace std;

long mdc( long a, long b ) { return b == 0 ? a : mdc( b, a%b ); }

struct MMC {
    auto operator()( long a, long b ) { return a*b/mdc(a,b); }    
};

struct BarraPesada {
    template <typename A, typename B>
    auto operator()( A a, B b ) { return a + b; } 
};

string ordena( string a, string b, string c, string d, string e, string f, string g, string h, string i, string j, string k, string l ) {
    std::array<string, 12> tab = { a, b, c, d, e, f, g, h, i, j, k, l };
    string result;
    
    std::sort( tab.begin(), tab.end() );
    for( auto itr = tab.begin(); itr != tab.end(); ++itr )
        result += *itr + " ";
    
    return result;    
}

int main() {
    using ::bind;
    // //1 ok
    // auto f1 = bind( mdc, 12 );
    // for( int i = 2; i <= 12; i++ )
    //     cout << f1( i ) <<  " ";
    
    // //2 ok
    // auto f2 = bind( mdc );
    // auto f1 = bind( f2, 12 );
    // for( int i = 2; i <= 12; i++ )
    //     cout << f1( i ) <<  " ";

    // //3
    // auto f2 = bind( mdc );
    // auto f1 = f2( 18 );
    // for( int i = 2; i <= 18; i++ )
    //     cout << f1( i ) <<  " ";

    // //4 ok
    // auto f2 = bind( []( int x, int y, int z ){ cout << x*y << (char) z << " " ; }, 10 );
    // for( int i = 0; i < 5; i++ )
    //     f2( i, ',' );
    
    // //5
    // auto f3 = bind( []( int x, int y, int z ){ cout << x*z << (char) y << " " ; } );
    // auto f1 = f3( 5, ';' );
    // for( int i = 0; i < 5; i++ )
    // f1( i );
    

    // //6 ok
    // MMC mmc;
    // auto f1 = bind( mmc, 6 );
    // for( int i = 2; i <= 12; i++ )
    // cout << f1( i ) << " ";
    

    // //7
    // MMC mmc; 
    // auto f2 = bind( mmc );
    // auto f1 = f2( 6 );
    // for( int i = 2; i <= 12; i++ )
    // cout << f1( i ) << " "; 
    

    //8  ok
    auto f1 = ::bind( ordena, "a", "b", "9", "6", "s", "2", "1", "0", "c", "d", "e" );
    cout << f1( "@" ) << endl;
    cout << f1( "~" );
    

    // //9 ok
    // auto f2=bind( BarraPesada() );
    // cout << f2( (string) "a", (string) "b" ) << endl;
    // cout << f2( 3.1, 4.3 );
    

    // //10 
    // BarraPesada barraMaisPesada;
    // auto f2=bind( barraMaisPesada );
    // auto f1s=f2( (string) "Hello, " );
    // auto f1f=f2( 3.14 );
    // cout << f1s( "world!" ) << endl;
    // cout << f1f( 4.3 );
    

    // //11 ok
    // string msg = "uma frase";
    // auto prefixo = ::bind( &string::substr, msg, 0 );
    // for( int i = 0; i < 5; i++ )
    // cout << prefixo( i + 1 ) << endl;
    
    return 0;
}