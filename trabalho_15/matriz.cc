#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sys/timeb.h>

using namespace std;

const int TAM = 1000; 

template <int L, int C>
class Matriz {
  public:
    Matriz() { inicializa(); }

  private:
    double m[L][C];

    auto geraMatriz() { double matriz[L][C]; return matriz; }
    
    void inicializa() {
      for( int i = 0; i < L; i++ )
        for( int j = 0; j < C; j++ )
          m[i][j] = rand() / (double) RAND_MAX;
    }
};

// template <int L, int LC, int C>
// Otimizador< ?? > operator * ( const Matriz<L,LC>& a, const Matriz<LC,C>& b ) {
// }

template <typename F>
class Apply {
public:
  Apply( F f ): f(f) {}
  
  template <int L, int C>
  Matriz<L,C> operator()( const Matriz<L,C>& m ) const {
  }
  
private:
  F f;
};

// double a[TAM][1];
// double b[1][TAM];
// double c[TAM][TAM];
// double t1[TAM][TAM];
// double t2[1][TAM];
// double res[TAM][TAM];

// template <int L, int C>
// void inicializa( double (&m)[L][C] ) {
//   for( int i = 0; i < L; i++ )
//     for( int j = 0; j < C; j++ )
//       m[i][j] = rand() / (double) RAND_MAX;
// }

template <int L, int CL, int C>
void mult( double (&res)[L][C], const double (&a)[L][CL], const double (&b)[CL][C] )
{
  for( int i = 0; i < L; i++ )
    for( int j = 0; j < C; j++ )
      res[i][j] = 0;
  
  for( int i = 0; i < L; i++ )
    for( int j = 0; j < C; j++ )
      for( int k = 0; k < CL; k++ )
	res[i][j] += a[i][k] * b[k][j];
}

int main() {
  // int opcao = 0;
  
  // inicializa( a );
  // inicializa( b );
  // inicializa( c );

  // cin >> opcao;
  
  // if( opcao == 0 ) {
  //   cout << "multiplicacao a * b * c" << endl;
  //   mult( t1, a, b );
  //   mult( res, t1, c );
  // }
  // else {
  //   cout << "multiplicacao a * (b * c)" << endl;
  //   mult( t2, b, c );
  //   mult( res, a, t2 );
  // }

  // Teste1 ---------------------------------------------------------------------------
  // int i = 0;
  // auto f0 = apply( [&i]( double x ) { return i++; } );
  // auto print = apply( []( double x ) { cout << x << " "; return x; } );
  // auto f2 = apply( []( double x ) { return x*x; } );
  Matriz<4,3> m;
  // m = f0( m );
  // print( m );
  // cout << endl;
  // m = f2( m );
  // print( m );
  // cout << endl;
  // cout << tamanho( m ) << endl;


// Teste2 ---------------------------------------------------------------------------
// Matriz<100,1> a;
// Matriz<1,100> b;
// auto x = a * b;
// auto y = b * a;
// cout << tamanho( x ) << endl;
// cout << tamanho( y ) << endl;


//Teste3
/* 
   Matriz<17,93> a, b;
  auto x = a + b;
  auto y = a + b + a + b;
  cout << tamanho( x ) << endl;
  cout << tamanho( y ) << endl;  
  return 0;
*/

//Teste4
/*     
Matriz<1000,1> a;
  Matriz<1,1000> b;
  Matriz<1000,1000> c;
  int tempo = 0, lapso1, lapso2;
 
  tempo = clock();
 
  Matriz<1000,1000> x1 = a * b;
  Matriz<1000,1000> x2 = x1 * c;
  cout << tamanho(x1) << endl;
  cout << tamanho(x2) << endl;
  lapso1 = clock() - tempo;
  tempo = clock();
  
  Matriz<1000,1000> y = a * b * c;
  lapso2 = clock() - tempo;
  cout << tamanho(y) << endl;
  if( lapso1/lapso2 > 10 ){
    cout << "Otimizou" << endl;
  }else{
    cout << "Não otimizou" << endl;
  }*/

  //Teste5
  /*  
  Matriz<997,1> a;
  Matriz<1,997> b;
  Matriz<997,997> c;
  int tempo = 0, lapso1, lapso2;
  tempo = clock();
  Matriz<997,997> x1 = a * b;
  Matriz<997,997> x2 = x1 * c;
  Matriz<997,997> t1 = a * (b + b);
  Matriz<997,997> x3 = x2 + t1 * c;
  cout << tamanho(x1) << endl;
  cout << tamanho(x2) << endl;
  cout << tamanho(t1) << endl;
  cout << tamanho(x3) << endl;
  lapso1 = clock() - tempo;
  tempo = clock();
  Matriz<997,997> y = a * b * c + a * (b + b) * c;
  lapso2 = clock() - tempo;
  cout << tamanho(y) << endl;
  if( lapso1/lapso2 > 10 )
    cout << "Otimizou" << endl;
  else
    cout << "Não otimizou" << endl;*/

  return 0;
}
