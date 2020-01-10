# Computação 3 - C++

Trabalhos realizados na disciplina Computação 3 no período 2019.2. Tal disciplina tem como foco a aprendizagem da linguagem C++ e a mesma é lecionada pelo professor Geraldo Zimbrão.

## Requisitos necessários

- Ambiente Linux
- Compilador g++

## Comandos para instalar o g++ no Linux Debian e derivados

    $ sudo apt-get update
    $ sudo apt-get install g++

## Enunciados dos trabalhos

### Trabalho 1 - Print com lista de tuplas

Faça uma template ou função "print" que recebe um lista de pares de tipos variáveis e as imprime. Isso pode ser feito usando pair ou tuple com dois elementos.

Um exemplo de declaração seria:

    int main() {
    
        print( { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );
        
        return 0;
    }

que deve imprimir:

    jan = 1
    2 = fev
    pi = 3.14

### Trabalho 2 - Criar uma classe Vetor Geometrico

Implementar uma classe template Vetor<n,T> onde T deve ser numérico. As operações básicas de vetores geométricos devem estar implementadas.

Um Vetor em duas dimensões seria Vetor<2,double>.

Criar os operadores de multiplicação, adição e subtração de vetores. Lembrando que existe produto escalar e produto vetorial. Criar também multiplicação e divisão por escalar.

Bole uma forma do código abaixo compilar:

    Vetor<2,double> v;
    v = 4.5, 6.1;

O produto escalar será feito através de '*', e o produto vetorial, através de '**'.

Esse operador '**' não existe em C++, mas ele pode ser simulado através de

    v = a ** b;

Para isso você deve redefinir o operador \'\*\' unário (ponteiro) para retornar uma classe intermediária (por exemplo, MeioDoProdutoVetorial) que contém o vetor b. Além disso, você deve redefinir o operador \'\*\' entre um Vetor e a classe MeioDoProdutoVetorial para realizar o produto vetorial.

Por último, crie o operador '<<' para imprimir.

Não colocar a main() no arquivo - o teste vai ter uma classe main.

### Trabalho 3 - Função apply

Crie uma função apply que recebe um array (pode ser vector também, na verdade qualquer tipo iterável) e uma função F, e retorna um vector cujo tipo base é definido pelo valor de retorno de F.

Exemplo:

    Entrada: vector<int> v{ 1, 2, 3, 4 }
    F = int sqr( int a ) { return a*a; }
    Saida: vector<int>, com os valores { 1, 4, 9, 16 } 

Se a função F for algo do tipo:

    string romanos( int n ) { // blablaba }

a saída será:

    vector<string> => { "I", "IV", "IX", "XVI" }

Exemplo de uso:

    vector<int> v{ 1, 3, 5 };

    vector<double> s = apply( v, sin );

### Trabalho 4 - Tarefa Stream

Implementar uma template que torne possível o seguinte tipo de código:

    int tab[10] =  { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
    vector<int> v{ 2 , 6, 8 };
    tab | []( int x ) { cout << x*x << endl; };
    tab | [ &v ]( int x ) { v.push_back( x ); };
    v | []( int x ) { cout << x*x << endl; };
    v | &print;

onde print é uma função que recebe um int.

    void print( int x ) { cout << x << " "; }

Esse operador será equivalente a:

    for_each( tab, tab+10,  []( int x ) { cout << x*x << endl; } );

### Trabalho 5 - Objeto com tipo variável

O objetivo desse trabalho é criar uma classe Var semelhante às variáveis do Javascript, que podem receber valores de vários tipos diferentes e durante a execução vão mudando internamente de tipo. Assim, uma variável pode ser criada como string e depois um double pode lhe ser atribuído, mudando o seu tipo. Se ela for um objeto, pode ter atributos que podem ser outras variáveis ou mesmo funções - semelhante ao que ocorre é Javascript. Nesse momento pense apenas na clareza, elegância e facilidade de implementação - ignore qualquer tentativa de otimização que possa surgir na sua mente durante a codificação.  Um exemplo de uso segue abaixo:

    #include <iostream>
    #include <string>
    #include <type_traits>
    #include <vector>

    using namespace std;

    int main() {
        Var a, b;
        
        cout << a << endl; // saída: "undefined"
        a = 1;
        cout << a << endl; // saída: 1
        a = "hello";
        cout << a << endl; // saída: hello
        
        a = newObject();
        a["atr"] = 9;
        a["metodo"] = []( auto x ){ return x*x; };
        b = 4;
        cout << a["metodo"]( b ) << endl; // Saída: 16;
    }

### Trabalho 6 - Tarefa Filter

Acrescentar na tarefa anterior o conceito de filter. Um filter é um predicado que retorna bool, e somente repassa para o próximo "pipe" os elementos que retornarem verdadeiro.

Usando o exempla tarefa Stream:

    v | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << endl; };

Irá imprimir apenas os números pares. s

### Trabalho 7 - Derivador Simbólico

Nesta tarefa você deve criar templates, funções e operadores para permitir a criação de um derivador simbólico. A idéia é que você possa definir uma fórmula para uma função que recebe um parâmetro x e essa função possa ser chamada através do método e (passando um valor para x), e calculando também o valor da derivada dela em relação a x através do método dx.

Exemplos:

    int main() {
    auto f = 3.0 * x * x;
    double v = 7;
    
    cout << "f(" << v << ")=" << f.e( v ) << ", f'(" << v << ")=" << f.dx( v ) << endl;
    }

Saida esperada:

    f(7)=147, f'(7)=42

Além das expressões com os operadores aritméticos simples +, -, * e /, implementar as funções exp, log, sin e cos. Lembre-se de usar a regra da cadeia para sin(f(x)) funcionar.

Redefina o operador "->*" para aceitar int, significando:

    x->*2 ==> x²

Gere um erro de compilação se "->*" for usado com um double: x->*0.5, por exemplo. A mensagem do erro deve ser: "Operador de potenciação definido apenas para inteiros".

### Trabalho 8 - Objeto com tipo variável - Arrays

Iremos evoluir a tarefa Objeto com tipo variável para lidar com arrays. Você poderá usar o seu código como base, ou alterar uma das soluções apresentadas no Moodle pelo professor. As seguintes alterações devem ser implementadas:

1. __Métodos de conversão__

    Definir os métodos isNumber(), asBool(), asString() etc. Serão métodos para teste e conversão de tipos. O comportamento esperado é o mesmo dos operadores em Javascript. Note que serão métodos da classe Var.

    Exemplos:

        Var b;
        for( b = 0; (b < 10).asBool(); b = b + 1 )
            cout << b <<  endl; 

2. __Arrays__

    A declaração de arrays será através da chamada da função newArray(). Um array é na verdade uma subclasse de Object, logo ele pode ter também propriedades indexadas por strings. Não há erros de limites de arrays para cima, apenas para índices negativos - nesse caso, o índice deve ser convertido em uma string e ser usado como nome de propriedade (ex: a[-1] se torna a["-1"]). Valores lidos que estão fora do limite do array são *undefined*. Se houver uma atribuição eles irão causar o redimensionamento do array. Arrays não diminuem de tamanho.

        Var a, b;
        a = newArray();
        a["sqr"] = []( int n ){ return n*n; }; 
        
        for( b = 0; (b < 10).asBool(); b = b + 1 )
            a[b] = a["sqr"]( b );

3. Métodos para Arrays:

    Os seguintes métodos devem ser implementados para arrays:

    1. __array.indexOf__( elemento ): retorna a posição do elemento no array, ou -1 se o elemento não está no array.
    2. __array.forEach__( function ): executa uma função para cada elemento do array. Deve retornar undefined.
    3. __array.filter__( function ): retorna um array contendo os elementos para os quais a função foi verdadeira.
    4. __array.map__( function ): retorna um array contendo a função aplicada a cada um dos elementos do array.

    Exemplos:

        Var pares = []( int n ){ return n%2 == 0; };
    
        a.filter( pares ).forEach( []( Var n ){ cout << n << " "; } );
        cout << endl;
    
        a.filter( pares ).map( [](auto x){ return x / 2 + 0.1; } ).forEach( []( Var n ){ cout << n << " "; } );  
        cout << endl;
    
        auto indexOf = []( const Var& array, Var valor ) {
            int n = 0;
            int pos = -1;
            
            array.forEach( [&n,&pos,valor](auto x) { 
                if( pos == -1 ) { 
                    if( (x == valor).asBool() ) 
                        pos = n; 
                    n++; 
                } 
            } ); 

            return pos;
        };
    
        cout << (indexOf( a, 36 ) == a.indexOf( 36 ) ) << endl;
        cout << indexOf( a.filter( pares ), "A" ) << endl;
        
        a[11] = 'A';
        cout << a.indexOf( "A" ) << endl;

### Trabalho 9 - Derivador Simbólico - Parte 2

Usando a solução da tarefa Derivador Simbólico, e efetuando as devidas correções e alterações, acrescente no derivador simbólico dois métodos: str e dx_str, com o cabeçalho abaixo:

    string str() const;
    string dx_str() const;

Esse métodos devem retornar a representação em string da função e de sua derivada. Não deve ser feita nenhuma simplificação, ou seja, saídas como (0\*x + 1\*1) são aceitáveis e devem ser mantidas.

Na saída substitua o operador "->*" por "^".

### Trabalho 10 - Expressões Lambda Simplificadas

Tente fazer expressões lambda que permitam o programa abaixo compilar (e executar corretamente):

    X x; // Declarar em algum lugar global.
    int tab[10] =  { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
    vector<int> v;
    tab | cout << x*x << '\n';
    tab | [ &v ]( int x ) { v.push_back( x ); };
    v | x % 2 == 0 | cout << x*x + 1 << '\n';
    v | x % 2 == 1 | x*x | cout << x << '\n';

### Trabalho 11 - Expressões Lambda Complicadas

Usando as Expressões Lambda Simplificadas da tarefa anterior, faça com que o código abaixo rode também:

    vector m< vector<int> >;
    m | x[0] != 0 | ( x | cout << x << '\n' );

Nesse caso m é um vetor de vetores, e então o primeiro x é um vetor. Logo, podemos escrever um *pipe* para ele.

E será necessário redefinir o operador "[]" para Expressões Lambda.

### Trabalho 12 - Derivador Simbólico - Parte 3

Usando o Derivador construído na parte 2, remova todos os parênteses desnecessários, e removendo multiplicação por 1, 0, adição com 0 e potenciação a 0 e 1.

Exemplos:

    Entrada: (x+3)->*2
    Saída: f(x) = (x+3)^2, f'(x) = 2*(x+3)

    Entrada: (x)->*3
    Saída: f(x) = x^3, f'(x) = 3*x^2

### Trabalho 13 - Classe Bind e função bind

Nesse trabalho você deve criar uma classe template Bind que recebe um functor/função e armazena esse functor, juntamente com possíveis parâmetros. Cada vez que uma função é ligada (binded) a um parâmetro, ela perde em n-aridade (ou seja, diminui o seu número de parâmetros). Ex: se uma função de dois parâmetros é ligada a um parâmetro, ela se torna uma nova função de apenas um parâmetro (o outro deve ser armazenado).

    int mdc( int a, int b ) { return b == 0 ? a : mdc( b, a%b ); }
    auto f = bind( mdc, 12 ); 
    // f é uma função de um parâmetro que calcula o mdc entre 12 e o parâmetro passado.  
    cout << f( 9 ) << endl; // deve imprimir 3, que é o mdc entre 12 e 9.

Outros exemplos de uso:

    void func4( int a, string b, int c, double d ) {
        // Faz algum processamento
    }

    int main() {
        auto f3 = bind( func4, 5 );

        cout << f3( "b", 7, 2.1 ) << endl;
        
        auto f2 = bind( f3, "b" );
        auto f2_a = f3( "b" );
        auto f2_b = f3( "b" );
        auto f1 = f3( "b", 8 );

        cout << f1( 1.1 ) << endl;
        cout << f2( 2, 1.1 ) << endl;
        cout << f2_a( 2, 1.1 ) << endl;
        cout << f2_b( 2, 1.1 ) << endl;
    }

Note que devemos declarar uma função auxiliar *bind*, embora poderia ser possível usar a sintaxe __*Bind{ func4, 1, "a" };*__ - só que não haverão testes neste sentido.

### Trabalho 16 - Lazy Streams

Tome como base a tarefa Filter (pode partir da solução dada no laboratório) para que os vetores não sejam instanciados. Para isso, você deve retornar um Iterator a cada operador "|". Esse iterator (que tem os métodos begin, end e "++") vai fornecer um elemento de cada vez para o próximo "pipe".

Rode com o mesmo exemplo:

    int main( int argc, char* argv[] ) {
        int tab[10] =  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        vector<int> v;
        
        tab | [ &v ]( int x ) { v.push_back( x ); };
        v | []( int x ) { return x % 2 == 0; } 
          | []( int x ) { return x*x / 10.0; } 
          | [] ( double x ) { cout << x << endl; };
        
        cout << endl;
        return 0;
    }

Crie também um gerador de intervalos, com a possibilidade de ter intervalos abertos (infinitos). Exemplo:

    Intervalo( 9, 99 )  | []( auto x ) { cout << x << " "; };  // Imprime de 9 a 98.

Crie uma classe que retorne o N-primeiros elementos do stream. O nome deve ser NPrimeiros.

    Intervalo( 0 ) | testa_se_e_primo | NPrimeiros( 10 ) | []( auto x ) { cout << x << " "; };

__testa_se_e_primo__ é uma função que testa se um número é primo (um predicado). NPrimeiros passa para o *pipe* seguinte os NPrimeiros elementos que passaram no predicado *testa_se_e_primo*.

Para começar a execução de um LazyStream é necessário colocar um Consumidor. Um Consumidor é uma função ou functor que retorna *void*.  

Assim o programa abaixo também deve ser válido:

    auto lazySt = Intervalo( 0 ) | testa_se_e_primo | NPrimeiros( 10 );
    lazySt | []( auto x ) { cout << x << " "; };

Apenas na segunda linha que os números devem ser gerados.