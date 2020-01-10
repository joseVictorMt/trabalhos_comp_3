#include <tuple>
#include <type_traits>
#include <functional>

struct PlaceHolder {};

PlaceHolder __;

auto extrai_calda = [](auto cabeca, auto... calda) { return tuple{ calda... }; };

template <typename Cabeca, typename... Calda>
auto calda(const std::tuple<Cabeca, Calda...>& t)
{
    return apply(extrai_calda, t);
}

template <typename ...Args1, typename ...Args2>
auto juntaTupla(tuple<Args1...> args1, tuple<Args2...> args2)
{
    if(tuple_size<decltype(args2)>::value == 0)
        return args1;

    auto arg1 = get<0>(args1);
    auto arg2 = get<0>(args2);
    if( is_same_v<decltype(arg1), PlaceHolder> && !is_same_v<decltype(arg2), PlaceHolder> ) {
        return tuple{ arg2, juntaTupla(calda(args1), calda(args2)) };
    } else if( is_same_v<decltype(arg2), PlaceHolder> ) {
        return tuple{ arg1, arg2, juntaTupla(calda(args1), calda(args2)) };
    } else if( !is_same_v<decltype(arg1), PlaceHolder> ) {
        return tuple{ arg1, juntaTupla(calda(args1), args2) };
    }
}

template <typename Args1, typename Args2>
auto defineArgumentos(tuple<Args1> args1, tuple<Args2> args2) {
    if( tuple_size<Args1>::value == 0 || (tuple_size<Args1>::value == 1 && get<0>(args1) == __) )
        return args2;
}

template <typename F, typename ...Args1>
class Bind
{
    private:
        F f;
        std::tuple<Args1...> args1;

    public:
        Bind( F f, Args1... args1 ): f(f), args1(args1...) {}
        Bind( F f, std::tuple<Args1...> args1 ): f(f), args1(args1) {}
    
        template <typename ...Args2>
        auto operator()( Args2... args2 ) {
            auto nova_tupla = defineArgumentos(args1, std::tuple{args2...});
            if constexpr( std::is_invocable_v< F, decltype(nova_tupla)> )
                return std::apply( f, nova_tupla );
            else
                return Bind<F,Args1...,Args2...>{ f, nova_tupla };
        }
};

template <typename F, typename ...Args1>
auto bind( F f, Args1... args1 ) {
    return Bind{ f, args1... };
}