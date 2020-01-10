class X
{
    public:

        double e( double v )
        {
            return v;
        }

        double dx( double v )
        {
            return 1;
        }
};

class Cte
{
    public:
        Cte( double c ): c(c){}

        double e( double v )
        {
            return c;
        }

        double dx( double v )
        {
            return 0;
        }

    private:
        double c;
};

template<typename F1, typename F2>
class Valores 
{
    public:
        Valores( F1 f1, F2 f2 ): f1(f1), f2(f2) {}

        virtual double e ( double v ) { return v; }

        virtual double dx( double v ) { return 1; }
    
    protected:
        F1 f1;
        F2 f2;
};

template<typename F1, typename F2>
class Soma : public Valores<F1, F2>
{
    public:
        Soma ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        double e( double v )
        {
            return this->f1.e( v ) + this->f2.e( v );
        }

        double dx( double v )
        {
            return this->f1.dx( v ) + this->f2.dx( v );
        }
};

template<typename F1, typename F2>
class Subtrai : public Valores<F1, F2>
{
    public:
        Subtrai ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        double e( double v )
        {
            return this->f1.e( v ) - this->f2.e( v );
        }

        double dx( double v )
        {
            return this->f1.dx( v ) - this->f2.dx( v );
        }
};

template<typename F1, typename F2>
class Multiplica : public Valores<F1, F2>
{
    public:
        Multiplica ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        double e( double v )
        {
            return this->f1.e( v ) * this->f2.e( v );
        }

        double dx( double v )
        {
            return this->f1.e( v ) * this->f2.dx( v ) + this->f1.dx( v ) * this->f2.e( v );
        }
};

template<typename F1, typename F2>
class Divide : public Valores<F1, F2>
{
    public:
        Divide ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        double e( double v )
        {
            return this->f1.e( v ) / this->f2.e( v );
        }

        double dx( double v )
        {
            return (this->f1.dx( v ) * this->f2.e( v ) - this->f1.e( v ) * this->f2.dx( v )) / ( pow( this->f2.e(v), 2 ) );
        }
};

template<typename F1, typename F2>
class Expoente : public Valores<F1, F2>
{
    public:
        Expoente ( F1 f1, F2 f2 ): Valores<F1, F2>( f1, f2 ) {}

        double e( double v )
        {
            return pow(this->f1.e( v ), this->f2.e( v ));
        }

        double dx( double v )
        {
            return this->f2.e(v) * pow(this->f1.e(v), this->f2.e(v) - 1) * this->f1.dx(v);
        }
};

template<typename F>
class Valor {
    public:
        Valor( F f ): f( f ) {}

        virtual double e( double v ) { return v; }

        virtual double dx( double v ) { return 1; }
    
    protected:
        F f;
};

template<typename F>
class Seno : public Valor<F> {
    public:
        Seno( F f ): Valor<F>( f ) {}

        double e( double v )
        {
            return sin( this->f.e(v) );
        }

        double dx( double v )
        {
            return cos( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Cosseno : public Valor<F> {
    public:
        Cosseno( F f ): Valor<F>( f ) {}

        double e( double v )
        {
            return cos( this->f.e(v) );
        }

        double dx( double v )
        {
            return -sin( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Exponencial : public Valor<F> {
    public:
        Exponencial( F f ): Valor<F>( f ) {}

        double e( double v )
        {
            return exp( this->f.e(v) );
        }

        double dx( double v )
        {
            return exp( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Logaritmica : public Valor<F> {
    public:
        Logaritmica( F f ): Valor<F>( f ) {}

        double e( double v )
        {
            return log( this->f.e(v) );
        }

        double dx( double v )
        {
            return 1/( this->f.e(v) ) * this->f.dx(v);
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
    static_assert(!std::is_same_v<double, F2> , "Operador de potenciação definido apenas para inteiros");
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

X x;