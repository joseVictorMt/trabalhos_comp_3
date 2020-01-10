#include <algorithm>
#include <type_traits>
#include <functional>

using namespace std;

auto Intervalo(int inicio = 0, int fim = 99) {
    vector<int> intervalo(fim - inicio, 0);
    
    int cont = inicio;
    for( auto i = begin(intervalo); i != end(intervalo); i++ )
        *i = cont++;

    return intervalo;
}

class NPrimeiros {
    public:
        NPrimeiros( int n ) : n(n) {}

        template< typename T >
        auto operator() (vector<T> v) {
            vector<T> ret(begin(v), begin(v) + n);
            return ret;
        }

    private:
        int n;
};

auto Impares = [](){
    return []( const vector<int>& v ) {
        vector<int> ret;
        int count = 0;
        
        for( auto x : v ) {
            if( x % 2 == 0 ) {
                ret.push_back(x);
                count++;
            }
        }

        ret.resize(count);
        return ret;
    };
};

template <typename V, typename F>
auto operator | ( const V& v, F func ) {
    if constexpr( is_invocable_v< F, decltype( *begin(v) ) > ) {
        if constexpr( is_same_v< bool, invoke_result_t< F, decltype( *begin( v ) ) > > ) {
            vector< decay_t< decltype( *begin( v ) ) > > res;

            for( auto x: v )
                if( invoke( func, x ) )
                    res.push_back( x );

            return res;
        }

        else if constexpr( is_same_v< void, invoke_result_t< F, decltype( *begin( v ) ) > > ) {
            for( auto x: v )
                func( x );
        }

        else {
            vector< decay_t< invoke_result_t< F, decltype( *begin( v ) ) > > > res;

            for( auto x: v )
                res.push_back( invoke( func, x ) );

            return res;
        }
    }
    
    else {
        vector<decay_t< decltype( *begin( v ) ) >> vetor(begin(v), end(v));
        return invoke( func, vetor );
    }
}