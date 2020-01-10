#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

class X
{
    public:

        string str()
        {
            return "x";
        }

        string dx_str()
        {
            return "1";
        }
};

class Cte
{
    public:
        Cte( int c ) : c(c){}
        Cte( double c ) : c(c){}

        string str()
        {
            double initpart;
            if( modf(c, &initpart) == 0 )
            {
                return  to_string((int) initpart);
            }
            else
            {
                ostringstream convStream;
                convStream << c << setprecision(2);
                return convStream.str();
            }
        }

        string dx_str()
        {
            return "0";
        }

    private:
        double c;
};

template<typename F1, typename F2>
class Valores 
{
    public:
        Valores( F1 f1, F2 f2 ): f1(f1), f2(f2) {}
    
    protected:
        F1 f1;
        F2 f2;
};

template<typename F1, typename F2>
class Soma : public Valores<F1, F2>
{
    public:
        Soma ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        string str()
        {
            return "((" + this->f1.str() + ")+(" + this->f2.str() + "))";
        }

        string dx_str()
        {
            return "((" + this->f1.dx_str() + ")+(" + this->f2.dx_str() + "))";
        }
};

template<typename F1, typename F2>
class Subtrai : public Valores<F1, F2>
{
    public:
        Subtrai ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        string str()
        {
            return "((" + this->f1.str() + ")-(" + this->f2.str() + "))";
        }

        string dx_str()
        {
            return "((" + this->f1.dx_str() + ")-(" + this->f2.dx_str() + "))";
        }
};

template<typename F1, typename F2>
class Multiplica : public Valores<F1, F2>
{
    public:
        Multiplica ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        string str()
        {
            return "((" + this->f1.str() + ")*(" + this->f2.str() + "))";
        }

        string dx_str()
        {
            return "((" + this->f1.dx_str() + ")*(" + this->f2.str() + 
            "))+((" + this->f1.str() + ")*(" + this->f2.dx_str() + "))";
        }
};

template<typename F1, typename F2>
class Divide : public Valores<F1, F2>
{
    public:
        Divide ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        string str()
        {
            return "((" + this->f1.str() + ")/(" + this->f2.str() + "))";
        }

        string dx_str()
        {
            return "((((" + this->f1.dx_str() + ")*(" + this->f2.str() + "))-((" + this->f1.str() + ")*(" + this->f2.dx_str() +
            ")))/((" + this->f2.str() + "^2)))";
        }
};

template<typename F1, typename F2>
class Expoente : public Valores<F1, F2>
{
    public:
        Expoente ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        string str()
        {
            return "(" + this->f1.str() + ")^" + this->f2.str() + "";
        }

        string dx_str()
        {
            int sub_exp = stoi(this->f2.str()) - 1;
            return "(" + this->f2.str() + "*(" + this->f1.dx_str() + ")*(" + this->f1.str() + ")^" + to_string(sub_exp) + ")";
        }
};

template<typename F>
class Valor {
    public:
        Valor( F f ): f( f ) {}
    
    protected:
        F f;
};

template<typename F>
class Seno : public Valor<F> {
    public:
        Seno( F f ): Valor<F>( f ) {}

        string str()
        {
            return "sin(" + this->f.str() + ")";
        }

        string dx_str()
        {
            return "cos(" + this->f.str() + ")*(" + this->f.dx_str() + ")";
        }
};

template<typename F>
class Cosseno : public Valor<F> {
    public:
        Cosseno( F f ): Valor<F>( f ) {}

        string str()
        {
            return "cos(" + this->f.str() + ")";
        }

        string dx_str()
        {
            return "-sin(" + this->f.str() + ")*(" + this->f.dx_str() +")";
        }
};

template<typename F>
class Exponencial : public Valor<F> {
    public:
        Exponencial( F f ): Valor<F>( f ) {}

        string str()
        {
            return "exp(" + this->f.str() + ")";
        }

        string dx_str()
        {
            return "(exp(" + this->f.str() + ")*" + this->f.dx_str() + ")";
        }
};

template<typename F>
class Logaritmica : public Valor<F> {
    public:
        Logaritmica( F f ): Valor<F>( f ) {}

        string str()
        {
            return "log(" + this->f.str() + ")";
        }

        string dx_str()
        {
            return "1/(" + this->f.str() + ")*(" + this->f.dx_str() + ")";
        }
};

template<typename F1, typename F2>
auto operator + ( F1 f1, F2 f2 )
{
    if constexpr(std::is_integral_v<F1> || std::is_floating_point_v<F1>)
        return Soma<Cte, F2>( f1, f2 );
    else if constexpr(std::is_integral_v<F2> || std::is_floating_point_v<F2>)
        return Soma<F1, Cte>( f1, f2 );
    else
        return Soma<F1, F2>( f1, f2 );
}

template<typename F1, typename F2>
auto operator - ( F1 f1, F2 f2 )
{
    if constexpr(std::is_integral_v<F1> || std::is_floating_point_v<F1>)
        return Subtrai<Cte, F2>( f1, f2 );
    else if constexpr(std::is_integral_v<F2> || std::is_floating_point_v<F2>)
        return Subtrai<F1, Cte>( f1, f2 );
    else
        return Subtrai<F1, F2>( f1, f2 );
}

template<typename F1, typename F2>
auto operator * ( F1 f1, F2 f2 )
{
    if constexpr(std::is_integral_v<F1> || std::is_floating_point_v<F1>)
        return Multiplica<Cte, F2>( f1, f2 );
    else if constexpr(std::is_integral_v<F2> || std::is_floating_point_v<F2>)
        return Multiplica<F1, Cte>( f1, f2 );
    else
        return Multiplica<F1, F2>( f1, f2 );
}

template<typename F1, typename F2>
auto operator / ( F1 f1, F2 f2 )
{
    if constexpr(std::is_integral_v<F1> || std::is_floating_point_v<F1>)
        return Divide<Cte, F2>( f1, f2 );
    else if constexpr(std::is_integral_v<F2> || std::is_floating_point_v<F2>)
        return Divide<F1, Cte>( f1, f2 );
    else
        return Divide<F1, F2>( f1, f2 );
}

template<typename F1, typename F2>
Expoente<F1, Cte> operator ->* ( F1 f1, F2 f2 )
{
    return Expoente<F1, Cte>( f1, f2 );
}

template<typename F1>
Seno< F1 > sin( F1 x ) {
    return Seno< F1 >( x );
}

template<typename F1>
Cosseno< F1 > cos( F1 x ) {
    return Cosseno< F1 >( x );
}

template<typename F1>
Exponencial< F1 > exp( F1 x ) {
    return Exponencial< F1 >( x );
}

template<typename F1>
Logaritmica< F1 > log( F1 x ) {
    return Logaritmica< F1 >( x );
}