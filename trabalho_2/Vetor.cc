#include <iostream>
#include <vector>

using namespace std;

template<int N = 3, typename T = double>
class Vetor {
    public:

        Vetor() {
            for( int i = 0; i < N; i++ ) {
                tab.push_back(0);
            }
            tab.resize(N);
        }

        // Classe de atribuição com vírgula.
        class AtribuicaoVirgula {
            public:
                AtribuicaoVirgula operator ,( T n ) {
                    *vetor = n;
                    return *this;
                }

            private:
                Vetor<N, T> *vetor;

                AtribuicaoVirgula( Vetor<N, T>* v ): vetor(v) {}

                // Declaro que a classe Vetor pode enxergar campos e métodos privados.
                friend class Vetor;
        };

        // Classe para pegar valor depois de *.
        class MeioDoProdutoVetorial {
            public:

                Vetor<N, T> obterVetor() const {
                    return *vetor;
                }

            private:
                Vetor<N, T> *vetor;

                MeioDoProdutoVetorial( Vetor<N, T>* v ): vetor(v) {}

                // Declaro que a classe Vetor pode enxergar campos e métodos privados.
                friend class Vetor;
        };

        // Insere novo valor ao vetor.
        void insereValor( T x ) {
            if(atual < N) {
                tab[ atual++ ] = x;
            } else {
                cout << "Não há mais espaço disponível no vetor." << endl;
            }
        }

        // Operador de atribuição.
        AtribuicaoVirgula operator = ( T n ) {
            insereValor(n);
            return AtribuicaoVirgula( this );
        }

        // retorna o vetor B.
        MeioDoProdutoVetorial operator * ( ) {
            return MeioDoProdutoVetorial( this );
        }

        // Pega o elemento do vetor na posição i.
        T getElement( unsigned int i ) const {
            return tab.at(i);
        }

        // Imprime os valores do vetor.
        void imprime( ostream& o ) const {
            for( auto x : tab ) {
                o << x << " ";
            }
        }

    private:
        vector<T> tab;
        unsigned atual = 0;
};

// Calcula a soma ou subtração entre dois vetores.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> somaSub( const Vetor<N, T1> a, const Vetor<N, T2> b, bool soma = true ) {
    Vetor<N, double> resultado;

    if(soma) {
        for( unsigned int i = 0; i < N; i++ ) {
            resultado.insereValor( a.getElement(i) + b.getElement(i) );
        }
    } else {
        for( unsigned int i = 0; i < N; i++ ) {
            resultado.insereValor( a.getElement(i) - b.getElement(i) );
        }
    }

    return resultado;
}

// Calcula a produto ou divisão por escalar.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> multiDivPorEscalar( double escalar, const Vetor<N, T2> b, bool multi = true ) {
    Vetor<N, double> resultado;

    if(multi) {
        for( unsigned int i = 0; i < N; i++ ) {
            resultado.insereValor( escalar * b.getElement(i) );
        }
    } else {
        for( unsigned int i = 0; i < N; i++ ) {
            resultado.insereValor( escalar / b.getElement(i) );
        }
    }

    return resultado;
}

// Calcula o produto escalar entre dois vetores.
template<int N = 3, typename T1 = double, typename T2 = double>
double produtoEscalar( const Vetor<N, T1> a, const Vetor<N, T2> b ) {
    double produto = 0;

    for( unsigned int i = 0; i < N; i++ ) {
        produto += ( a.getElement(i) * b.getElement(i) );
    }

    return produto;
}

// Calcula o produto vetorial entre dois vetores.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> produtoVetorial( const Vetor<N, T1> a, const Vetor<N, T2> b ) {
    Vetor<N, double> produto;
    double temp;

    // Eixo x
    temp = (a.getElement(1) * b.getElement(2)) - (a.getElement(2) * b.getElement(1));
    produto.insereValor( temp );
    // Eixo Y
    temp = (a.getElement(2) * b.getElement(0)) - (a.getElement(0) * b.getElement(2));
    produto.insereValor( temp );
    // Eixo z
    temp = (a.getElement(0) * b.getElement(1)) - (a.getElement(1) * b.getElement(0));
    produto.insereValor( temp );

    return produto;
}

// Operador de + para somar vetores.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator + ( const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return somaSub( a, b );
}

// Operador de - para subtrair vetores.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator - ( const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return somaSub( a, b, false );
}

// Operador de * para produto por escalar sendo o escalar o primeiro parâmetro.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator * ( const double a, const Vetor<N, T2> b) {
    return multiDivPorEscalar( a, b );
}

// Operador de * para produto por escalar sendo o escalar o segundo parâmetro.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator * ( const Vetor<N, T2> b, const double a) {
    return multiDivPorEscalar( a, b );
}

// Operador de / para divisão por escalar sendo o escalar o primeiro parâmetro.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator / ( const double a, const Vetor<N, T2> b) {
    return multiDivPorEscalar( a, b, false );
}

// Operador de / para divisão por escalar sendo o escalar o segundo parâmetro.
template<int N = 3, typename T1 = double, typename T2 = double>
Vetor<N, double> operator / ( const Vetor<N, T2> b, const double a) {
    return multiDivPorEscalar( a, b, false );
}

// Operador de * para produto escalar.
template<int N = 3, typename T1 = double, typename T2 = double>
double operator * ( const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return produtoEscalar( a, b );
}

// Operador de * para produto Vetorial.
template<int N = 3, typename T1 = double, typename T2=double>
Vetor<N, double> operator * ( const Vetor<N, T1> a, const typename Vetor<N, T2>::MeioDoProdutoVetorial b) {
    return produtoVetorial( a, b.obterVetor() );
}

// Adapta o operador << para imprimir elementos de um vetor.
template<int N = 3, typename T = double>
ostream& operator << ( ostream& o, const Vetor<N, T>& v ) {
    v.imprime(o);
    return o;
}

int main( int argc, char *argv[] ) {
    Vetor<3, double> v1, v2;

    v1 = 2.1, 4.2, 7.2;
    v2 = 87, 59, 34;

    cout << v1 + v2 << endl;

    cout << v1 - v2 << endl;

    cout << 3 * v1 << endl;

    cout << v1 * 3 << endl;

    cout << 3 / v1 << endl;

    cout << v1 / 3 << endl;

    cout << v1 * v2 << endl;

    cout << v1 ** v2 << endl;

    return 0;
}
