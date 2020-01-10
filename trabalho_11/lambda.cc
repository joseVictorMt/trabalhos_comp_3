#include <iostream>
#include <type_traits>
#include <functional>
#include <vector>

using namespace std;

template <typename Functor>
class Lambda
{
    public:
        Lambda( Functor f ): f(f) {}

        template <typename V>
        invoke_result_t< Functor, V > operator()( V v ) const {
            return f( v );
        }

        auto operator[] ( auto linha ) {
            return [linha]( auto v ) { return v[linha]; };
        }

    private:
        Functor f;
};

auto aux = []( auto v ){ return v; };

Lambda x{aux};

template <typename A>
auto operator << ( ostream& o, Lambda<A> opr ) { 
    return Lambda{ [&o,opr]( auto v ) -> decltype( o << opr(v) )
                { return o << opr(v); } };
}

#define DEFINE_LAMBDA( opr )                                           \
template <typename A, typename B>                                      \
auto operator opr ( Lambda<A> a, Lambda<B> b ) {                       \
    return Lambda{ [a,b]( auto v ) -> decltype( a(v) opr b(v) )        \
                 { return a(v) opr b(v); } };                          \
}                                                                      \
                                                                       \
template <typename A, typename B>                                      \
auto operator opr ( Lambda<A> a, B b ) {                               \
    return Lambda{ [a,b]( auto v ) -> decltype( a(v) opr b    )        \
                 { return a(v) opr b; } };                             \
}                                                                      \
                                                                       \
template <typename A, typename B>                                      \
auto operator opr ( A a, Lambda<B> b ) {                               \
    return Lambda{ [a,b]( auto v ) -> decltype( a opr b(v)    )        \
                { return a opr b(v); } };                              \
}                                                                      \

DEFINE_LAMBDA( + )
DEFINE_LAMBDA( - )
DEFINE_LAMBDA( * )
DEFINE_LAMBDA( / )
DEFINE_LAMBDA( % )
DEFINE_LAMBDA( == )
DEFINE_LAMBDA( != )
DEFINE_LAMBDA( << )

// Condição especial para uma expressão 'x[0] != 0'
template <typename A, typename B>
auto operator != ( A a, B b ) { 
    return Lambda{ [a,b]( auto v ) { return a(v) != b; } };
}

// Tratador para operador | entre parenteses
template <typename A, typename B>
auto operator | ( Lambda<A> a, Lambda<B> b ) { 
    return Lambda{ [a,b]( auto v ) { a(v) | b; } };
}

template<typename T, typename F>
auto operator | ( const T& iterable, F function ) {
    if constexpr ( is_same_v< bool, invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
        vector< decay_t< decltype( *begin(iterable) ) > > result;

        for( auto element : iterable ) 
            if( invoke( function, element ) )
                result.push_back( element );
        
        return result;
    }
    else if constexpr ( is_same_v< void, invoke_result_t< F, decltype( *begin(iterable) ) > > ||
                        is_reference_v< invoke_result_t< F, decltype( *begin(iterable) ) > > ) {
        for( auto element : iterable ) 
            invoke( function, element );
    }
    else {
        std::vector< decay_t< invoke_result_t< F, decltype( *begin(iterable) ) > > > result;

        for( auto element : iterable ) 
            result.push_back( invoke( function, element ) );
        
        return result;
    }
}

int main() {

    // Teste 1 ------------------------------------------------
    // vector< vector<int> > m{ { 1, 2, 3 },{ 0, 3, 7 },{ 1, 3 } };
    // m | [](auto v) { return v[0] != 0; } | [](auto v) { (v + 7) | cout << x << ' '; };
    // // Teste 2 ------------------------------------------------
    // vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
    // m | []( auto v ) { return v[0] != 0; } | ( x | cout << x << ' ' );
    // // Teste 3 ------------------------------------------------
    // vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
    // m | x[0] != 0 | ( x | cout << x << ' ' ); 
    // // Teste 4 ------------------------------------------------
    // vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
    // m | x[0] != 0 | ( ( x + 7 )| cout << x << ' ' );
    
    return 0;
}