#include "tipo_var.cc"

int main(void)
{
    // Var a, b = 10;
    // cout << a << " " << b << endl;
    // a = 3.14;
    // b = "uma string";
    // cout << a << " " << b << endl;

    //____________________________________________________

    // Var a[5] = { true, 'X', 2, 2.1, "abracadabra" };
    // Var b = 200, c = 300.1, d = "palavra ";
    // for( auto& x: a ) {
    //     cout << x+b << "," << x+c << "," << x+d << "," << 3 / x << "," << 1.1 * x << ","
    //     << (x && true) << "," << (x && false) <<  "," << (b >= x) << "," << (x < d) << endl;
    // }

    // ____________________________________________________

    // Var a;
    // a = 3;
    // cout << a["mes"] << endl;

    // ____________________________________________________

    // Var a;
    // a = newObject();
    // cout << a(5) << endl;

    // ____________________________________________________

    // Var a, b;
    // a = newObject();
    // b = "José Maria";
    // a["nome"] = b;
    // a["nascimento"] = 1998;
    // b = "Maria José";
    // try {
    //     a.print( cout );
    // } catch( Var::Erro e ) {
    //     cout << "Erro fatal: " << e() << endl;
    // }
    // cout << a << " " << a["nome"] << " " << a["nascimento"] << endl;

    // ____________________________________________________

    // Var a, b;
    // a = 10.1;
    // b = []( auto x ){ return x + x; };
    // cout << b( a ) << " ";
    // cout << b( "oba" ) << " ";
    // cout << b( 'X' ) << " ";
    // cout << b( true );

    // ____________________________________________________

    // Var a = newObject();
    // Var b = "José", c = "Maria";
    // a["nome"] = b + ' ' + c;
    // a["idade"] = []( auto v ) { return 2019 - v["nascimento"]; };
    // a["nascimento"] = 1990;
    // a["print"] = &print;
    // b = a;
    // imprime( a );
    // a["nascimento"] = 2001;
    // imprime( a );
    // imprime( b );

    // ____________________________________________________

    // Var a, b;
    // a = newObject();
    // a["init"] = []( auto x )
    //     {
    //         x["nome"] = "Manoel";
    //         x["idade"] = []( auto v ) { return 2019 - v["nascimento"]; };
    //         x["nascimento"] = 1987;
    //         x["print"] = &print;
    //         return x;
    //     };
    // b = [a]( auto x ){ return x( a ); };
    // a["funcao"] = b;
    // b = &print;
    // a["funcao"]( a["init"] );
    // a["atr"] = a["init"]( newObject() );
    // a["funcao"]( b );
    // imprime( a["atr"] );

    return 0;
}