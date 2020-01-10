#include <iostream>
#include <vector>

using namespace std;

template <class C1, class C2, class Function>
void implementFunction(C1& input, C2& output, Function fn) {
    for(auto x : input) {
        output.push_back(fn(x));
    }

    output.resize(input.size());
}

template <
    typename T,
    typename A,
    template <typename, typename> class Vector,
    class Function
>
vector <typename result_of<Function(T)>::type> apply(
    Vector<T, A> vetor,
    Function fn)
{
    vector<typename result_of<Function(T)>::type> result;

    implementFunction(vetor, result, fn);

    return result;
}

template <typename T, class Function>
vector <typename result_of<Function(T)>::type> apply(
    T *vetor,
    Function fn)
{
    vector<T> values (vetor, vetor + sizeof(T) + 1);

    vector<typename result_of<Function(T)>::type> result;

    implementFunction(values, result, fn);

    return result;
}

template<typename T, class Function>
vector <typename result_of<Function(T)>::type> apply(
    initializer_list<T> list,
    Function fn)
{
    vector <typename result_of<Function(T)>::type> result;

    implementFunction(list, result, fn);

    return result;
}
