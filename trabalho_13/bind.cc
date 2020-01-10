#include <type_traits>
#include <functional>

using std::is_invocable_v, std::invoke, std::is_class_v;

template<typename Func, typename FuncBase, typename ...Params>
class Bind {
    public:
        Bind( Func f, FuncBase a , Params ...params ) : f(f) {}

        template<typename ...Args>
        auto operator() (Args ...args) {
            if constexpr(is_invocable_v<FuncBase, Params..., Args...>)
                return invoke(f, args...);
            else
                return [this, args...](auto ...outros){ return invoke(f, args..., outros...); };
        }

        Func recuperaFunc() const { return f; }

    private:
        Func f;
};

template<typename Func, typename FuncBase>
auto bind(Bind<Func, FuncBase> f, auto ...args) {
    auto rec = f.recuperaFunc();
    auto func = [rec, args...](auto... params){ return invoke(rec, args..., params...); };
    Bind b{func, f, args...};
    return b;
}

template<typename Func>
auto bind(Func f, auto ...args) {
    if constexpr (is_class_v<Func>) {
        auto func = [&f, args...](auto... params){ return invoke(f, args..., params...); };
        Bind b{func, f, args...};
        return b;
    } else {
        auto func = [f, args...](auto... params){ return invoke(f, args..., params...); };
        Bind b{func, f, args...};
        return b;
    }
}
