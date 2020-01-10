#include "derivador.cc"

int main( void )
{
    // // Teste 1
    // auto f = 3.0 + x + x;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 2
    // auto f = x*x;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 3
    // auto f = x -(x - 5.0);
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 4
    // auto f = (x+7.0)*(x-7.0);
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 5
    // auto f = x->*3;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 6
    // auto f = (7.0*x+5.0)->*3;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 7
    // auto f = sin( x + 5 );
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 8
    // auto f = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 9
    // auto f = exp( x * log( x - 8.0 ) + 1.0 );
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 10
    // auto f = exp( (x + 1.0)*(x - 1.0) )->*2;
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // // Teste 11
    // auto f = log( x->* 4 + 8.0 ) * sin( cos( x / 3.14 ) );
    // cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    // Teste 12
    auto f = log( (8.1/(x+1.0)->*3 - 9.2 ) * (x + 3) * sin( cos( x / 3.14 ) + x ));
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

    return 0;
}