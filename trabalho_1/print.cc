#include <initializer_list>
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

using namespace std;

class Pair {
    public:
        template <typename A, typename B>
        Pair( A a, B b ) {
            first = converter(a);
            second = converter(b);
        }

        // Mensageiro de atributos, no formato pair.
        pair<string, string> getElements() {
            return make_pair(first, second);
        }

    private:
        string first, second;

        // Conversor de valores em string.
        template <typename T>
        string converter( T param ) {
            // Aqui é criada a stream usada para a conversão.
            ostringstream convStream;
            // Essa stream recebe o parâmetro que será convertido.
            convStream << param;
            // O retorno passa ser o parâmetro no formato string.
            return convStream.str();
        }
};

// Imprime um ou mais objetos do tipo Pair.
void print( initializer_list<Pair> lista ) {
    for( auto elem : lista ) {
        cout << elem.getElements().first
             << " = "
             << elem.getElements().second
             << endl;
    }
}

int main() {

    Pair p( "1", 2 );

    print( { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

    return 0;
}
