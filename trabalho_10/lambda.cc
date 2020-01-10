#include <algorithm>
#include <functional>
#include <type_traits>

class X {
    public:
        template <typename K>
        K operator() ( const K valor ) const {
            return valor;
        }
};

template <typename T> 
class Cte {
    public:
        Cte( const T& valor ) : valor( valor ) {}

        template<typename K>
        T operator() ( K ignorar ) const {
            return valor;
        } 

    private:
        T valor;
};

// ------------------------------------- Soma -------------------------------------

template <typename Op1, typename Op2> 
class Soma {
    public:
        Soma( Op1 a, Op2 b ) : a(a), b(b) {}

        template<typename K>
        K operator() ( K valor ) {
            return a( valor ) + b( valor );
        }

    private:
        Op1 a;
        Op2 b;
};

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op1>, int> = 0>
auto operator + ( Op1 a, Op2 b ) {
    return Soma< Cte<Op1>, Op2 >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op2>, int> = 0>
auto operator + ( Op1 a, Op2 b ) {
    return Soma< Op1, Cte<Op2> >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<!std::is_arithmetic_v<Op2> && !std::is_arithmetic_v<Op2>, int> = 0>
auto operator + ( Op1 a, Op2 b ) {
    return Soma< Op1, Op2 >( a, b );
}

// ------------------------------------- Produto -------------------------------------

template <typename Op1, typename Op2> 
class Produto {
    public:
        Produto( Op1 a, Op2 b ) : a(a), b(b) {}

        template<typename K>
        K operator() ( K valor ) {
            return a( valor ) * b( valor );
        }

    private:
        Op1 a;
        Op2 b;
};

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op1>, int> = 0>
auto operator * ( Op1 a, Op2 b ) {
    return Produto< Cte<Op1>, Op2 >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op2>, int> = 0>
auto operator * ( Op1 a, Op2 b ) {
    return Produto< Op1, Cte<Op2> >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<!std::is_arithmetic_v<Op1> && !std::is_arithmetic_v<Op2>, int> = 0>
auto operator * ( Op1 a, Op2 b ) {
    return Produto< Op1, Op2 >( a, b );
}

// ------------------------------------- Modulo -------------------------------------

template <typename Op1, typename Op2> 
class Modulo {
    public:
        Modulo( Op1 a, Op2 b ) : a(a), b(b) {}

        template<typename K>
        K operator() ( K valor ) {
            return a( valor ) % b( valor );
        }

    private:
        Op1 a;
        Op2 b;
};

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_integral_v<Op1>, int> = 0>
auto operator % ( Op1 a, Op2 b ) {
    return Modulo< Cte<int>, Op2 >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_integral_v<Op2>, int> = 0>
auto operator % ( Op1 a, Op2 b ) {
    return Modulo< Op1, Cte<int> >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<!std::is_integral_v<Op1> && !std::is_integral_v<Op2>, int> = 0>
auto operator % ( Op1 a, Op2 b ) {
    return Modulo< Op1, Op2 >( a, b );
}

// ------------------------------------- Igual -------------------------------------

template <typename Op1, typename Op2> 
class Igual {
    public:
        Igual( Op1 a, Op2 b ) : a(a), b(b) {}

        template<typename K>
        bool operator() ( K valor ) {
            return a( valor ) == b( valor );
        }

    private:
        Op1 a;
        Op2 b;
};

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op1>, int> = 0>
auto operator == ( Op1 a, Op2 b ) {
    return Igual< Cte<Op1>, Op2 >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<std::is_arithmetic_v<Op2>, int> = 0>
auto operator == ( Op1 a, Op2 b ) {
    return Igual< Op1, Cte<Op2> >( a, b );
}

template <typename Op1, typename Op2,
typename std::enable_if_t<!std::is_arithmetic_v<Op1> && !std::is_arithmetic_v<Op2>, int> = 0>
auto operator == ( Op1 a, Op2 b ) {
    return Igual< Op1, Op2 >( a, b );
}

// ------------------------------------- Ostream -------------------------------------

template <typename Ost, typename Op>
class Ostream {
    public:
        Ostream( Ost &o, Op opr ) : o(o), opr(opr) {}

        template<typename K>
        void operator() ( K valor ) {
            o << opr( valor ) << buffer;
        }

        void inclui( const char c ) {
            buffer += c;
        }

    private:
        Ost &o;
        Op opr;
        std::string buffer = "";
};

template <typename Op1, typename Op2>
auto operator << ( std::ostream& o, Soma< Op1, Op2 > b ) {
    return Ostream< std::ostream, Soma< Op1, Op2 > >( o, b );
}

template <typename Op1, typename Op2>
auto operator << ( std::ostream& o, Produto< Op1, Op2 > b ) {
    return Ostream< std::ostream, Produto< Op1, Op2 > >( o, b );
}

template <typename Op1, typename Op2>
auto operator << ( std::ostream& o, Modulo< Op1, Op2 > b ) {
    return Ostream< std::ostream, Modulo< Op1, Op2 > >( o, b );
}

template <typename Op1, typename Op2>
auto operator << ( std::ostream& o, Igual< Op1, Op2 > b ) {
    return Ostream< std::ostream, Igual< Op1, Op2 > >( o, b );
}

auto operator << ( std::ostream& o, X x ) {
    return Ostream< std::ostream, X >( o, x );
}

template <typename Ost,
typename std::enable_if_t<std::is_member_function_pointer_v<decltype(&Ost::inclui)>, int> = 0>
auto operator << ( Ost a, const char c ) {
    a.inclui(c);
    return a;
}

// ------------------------------------- Filtro -------------------------------------

template <typename Vector, typename Func>
auto operator | ( const Vector& v , Func f ) {
    if constexpr ( std::is_void_v< std::invoke_result_t<Func, decltype(*std::begin(v))> > )
    {
        std::for_each( std::begin(v), std::end(v), [&f]( auto x ){ std::invoke( f, x ); } );
    }
    else if constexpr ( std::is_same_v< std::invoke_result_t<Func, decltype(*std::begin(v))>, bool > )
    {
        std::vector< std::decay_t<decltype( *std::begin(v) )> > rst;
        int count = 0;
        std::for_each(
            std::begin(v),
            std::end(v),
            [&f, &rst, &count]( auto x ){ if(std::invoke( f, x )) { rst.push_back(x); count++; } } );
        rst.resize(count);
        return rst;
    }
    else
    {
        std::vector< std::decay_t< std::invoke_result_t<Func, decltype(*std::begin(v))> > > rst;
        int count = 0;
        std::for_each(
            std::begin(v),
            std::end(v),
            [&f, &rst, &count]( auto x ){ rst.push_back(std::invoke( f, x )); count++; } );
        rst.resize(count);
        return rst;
    }
}

X x;