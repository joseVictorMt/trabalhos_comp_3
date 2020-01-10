#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <type_traits>
#include <variant>

using namespace std;

class Var
{
    public:

        // ------------------------------- Classes Internas -------------------------------

        class Erro: public exception
        {
            public:
                Erro( string msg ): msg( msg ) {}

                string operator()() { return msg; }

                string what() { return msg; }
            
            private:
                string msg;
        };

        class Undefined {};

        class Object
        {
            public:
                virtual ~Object() {}
        
                virtual void print( ostream& o ) const
                { 
                    o << "object";
                }
        
                virtual Var func( const Var& arg ) const
                { 
                    throw Erro( "Essa variável não pode ser usada como função" ); 
                }

                virtual void forEach( const Var& f ) const
                {
                    throw Erro( "Essa variável não é um array de objetos" );
                }

                virtual void filter( const Var& f )
                {
                    throw Erro( "Essa variável não é um array de objetos" );
                }
        
                virtual Var indexOf( const string elem ) const
                {
                    throw Erro( "Essa variável não é um array de objetos" );
                }

                virtual Var& lvalue( const Var& st )
                {
                    switch( st.v.index() )
                    {
                        case STRING:
                            return atr[ get<STRING>( st.v ) ];
                        
                        default:
                            throw Erro( "O objeto deve ser indexado com string" );
                    }
                }

                Var rvalue( const string& st ) const
                { 
                    if( auto x = atr.find( st ); x != atr.end() )
                        return x->second;
                    else
                        return Var();
                }
        
            private:
                map<string,Var> atr; 
        };

        class Array: public Object
        {
            public:
                Var& lvalue( const Var& i )
                {
                    switch( i.v.index() )
                    {
                        case INT: return arr[ to_string(get<INT>(i.v)) ];
                        case DOUBLE: return arr[ to_string(get<DOUBLE>(i.v)) ];
                        case STRING: return arr[ get<STRING>(i.v) ];

                        default:
                            throw Erro( "Tipo inválido para referenciar element de array" );
                    }
                }

                void forEach( const Var& f ) const
                {
                    for_each( arr.begin(), arr.end(), [ f ]( auto x ){ get<OBJECT>( f.v )->func( x.second ); } );
                }

                void filter( const Var& f )
                {
                    for( auto x : arr )
                    {
                        if ( !get<OBJECT>( f.v )->func( x.second ).asBool() )
                        {
                            arr.erase( x.first );
                        }
                    }
                }

                Var indexOf( const string elem ) const
                {
                    for( auto x : arr )
                    {
                        if( elem.compare(x.second.asString()) == 0 )
                            return x.first;
                    }
                    return Var();
                }

            private:
                map<string, Var>arr;
        };
    
    private:

        template <class F>
        class Function: public Object
        {
            public:
                Function( F f ): f(f) {}

                void print( ostream& o ) const
                {
                    o << "function"; 
                }
            
                Var func( const Var& arg ) const
                {
                    if constexpr ( is_invocable_v<F, Var> )
                    {
                        if constexpr ( is_void_v<invoke_result_t<F, Var>> )
                        {
                            invoke( f, arg );
                            return Var();
                        }
                        else
                        {
                            return invoke( f, arg );
                        }
                    }
                    else if constexpr (is_invocable_v<F, int>)
                    {
                        if constexpr ( is_void_v<invoke_result_t<F, int>> )
                        {
                            invoke( f, get<INT>( arg.v ) );
                            return Var();
                        }
                        else
                        {
                            return invoke( f, get<INT>( arg.v ) );
                        }
                    } 
                    else
                    {
                        throw Erro( "Função não invocável" );
                    }
                }
        
            private:
                F f;
        };

        // ------------------------------- Variant -------------------------------

        enum { UNDEFINED, BOOL, CHAR, INT, DOUBLE, STRING, OBJECT };

        typedef variant<Undefined,bool,char,int,double,string,shared_ptr<Object>> Variant;

    public:

        // ----------------------- Postergador de operador -----------------------

        class DuplaExclamacao {
            public:
                bool obterResultado() const
                {
                    switch( var.index() )
                    {
                        case UNDEFINED: return false;
                        case BOOL: return get<BOOL>( var ) ? true : false;
                        case INT: return get<INT>( var ) != 0 ? true : false;
                        case CHAR: return get<CHAR>( var ) > 0 ? true : false;
                        case DOUBLE: return get<DOUBLE>( var ) != 0.0 ? true : false;
                        case STRING: return get<STRING>( var ).compare("") == 0 ? false : true;
                        case OBJECT: return true;
            
                        default:
                            throw Erro( "Tipo indefinido (bug)" );
                    }
                }

            private:
                Variant var;
                
                DuplaExclamacao( Variant var ): var( var ) {}
                
                friend class Var;
        };

        // ------------------------------- Construtores -------------------------------
  
        Var(): v() {}
        Var( const char* st ): v( string(st) ) {}
        Var( bool v ): v(v) {}
        Var( char v ): v(v) {}
        Var( int v ): v(v) {}
        Var( double v ): v(v) {}
        Var( string v ): v(v) {}
        Var( Array* v ): v( shared_ptr<Object>( v ) ) {}
        Var( Object* v ): v( shared_ptr<Object>( v ) ) {}

        template <typename T>
        Var( T func ): v( shared_ptr<Object>( new Function( func ) ) ) {}

        // ---------------------------- Operadores Internos ----------------------------
    
        Var operator()( Var arg ) const
        {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não pode ser usada como função" );

            return get<OBJECT>( v )->func( arg );
        }

        Var& operator[]( const Var& elem )
        {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );

            return get<OBJECT>( v )->lvalue( elem );
        }

        Var operator[]( const string& st ) const
        {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );
                
            return get<OBJECT>( v )->rvalue( st );
        }

        DuplaExclamacao operator ! () const
        {
            return DuplaExclamacao( this->v );
        }

        // ------------------------------- Métodos -------------------------------

        void print( ostream& o ) const
        {
            switch( v.index() )
            {
                case UNDEFINED: o << "undefined"; break;
                case BOOL: o << (get<BOOL>( v ) ? "true" : "false"); break;
                case CHAR: o << get<CHAR>( v ); break;
                case INT: o << get<INT>( v ); break;
                case DOUBLE: o << get<DOUBLE>( v ); break;
                case STRING: o << get<STRING>( v ); break;
                case OBJECT: get<OBJECT>( v )->print( o ); break;
    
                default:
	                throw Erro( "Tipo indefinido (bug)" );
            }
        }

        string asString() const
        {
            switch( v.index() )
            {
                case UNDEFINED: return "undefined";
                case BOOL: return get<BOOL>( v ) ? "true" : "false";
                case CHAR: return string("") + get<CHAR>( v );
                case INT: return to_string( get<INT>( v ) );
                case DOUBLE: return to_string( get<DOUBLE>( v ) );
                case STRING: return get<STRING>( v );
                case OBJECT: return "object";

                default:
	                throw Erro( "Tipo indefinido (bug)" );
            }
        }

        bool asBool() const
        {
            switch( v.index() )
            {
                case UNDEFINED: return false;
                case BOOL: return get<BOOL>( v ) ? true : false;
                case INT: return get<INT>( v ) != 0 ? true : false;
                case CHAR: return get<CHAR>( v ) > 0 ? true : false;
                case DOUBLE: return get<DOUBLE>( v ) != 0.0 ? true : false;
                case STRING: return get<STRING>( v ).compare("") == 0 ? false : true;
                case OBJECT: return true;
    
                default:
	                throw Erro( "Tipo indefinido (bug)" );
            }
        }

        bool isNumber() const
        {
            string temp;

            switch( v.index() ) {
                case INT: return true;
                case BOOL: return true;
                case DOUBLE: return true;
                case CHAR:
                    return isdigit(get<CHAR>( v )) ? true : false;
                case STRING:
                    temp = get<STRING>( v );
                    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());

                    if( regex_match( temp, regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?") ) )
                    {
                        return true;
                    }
                    else if( regex_match( temp, regex("((\\+|-)?[[:digit:]]+)?") ) )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                case UNDEFINED: return false;
                case OBJECT: return false;

                default:
	                throw Erro( "Tipo indefinido (bug)" );
            }
        }

        void forEach( const Var& f ) const
        {
            if ( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );
            
            if ( f.v.index() != OBJECT )
                throw Erro( "O parâmetro passado não é um objeto" );
            
            get<OBJECT>( v )->forEach( f );
        }

        Var filter( const Var& f )
        {
            if ( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );
            
            if ( f.v.index() != OBJECT )
                throw Erro( "O parâmetro passado não é um objeto" );

            get<OBJECT>( v )->filter( f );

            return Var( *this );
        }

        template<typename T>
        Var indexOf( T elem )
        {
            if ( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );
            Var temp = elem;
            return get<OBJECT>( v )->indexOf( temp.asString() );
        }

        // -------------------------------------- Operações --------------------------------------
    
        static constexpr int tipo( int a, int b ) { return (a << 4) | b; }
  
        static Var sel_soma( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( INT, INT ): return get<INT>( a.v ) + get<INT>( b.v );
                case tipo( CHAR, CHAR ): return string("") + get<CHAR>( a.v ) + get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) + get<DOUBLE>( b.v );
                case tipo( STRING, STRING ): return get<STRING>( a.v ) + get<STRING>( b.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) + get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) + get<DOUBLE>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) + get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) + get<INT>( b.v );

                case tipo( STRING, CHAR ): return get<STRING>( a.v ) + get<CHAR>( b.v );
                case tipo( CHAR, STRING ): return get<CHAR>( a.v ) + get<STRING>( b.v );
      
                default:
                    return Var();
            }
        }

        static Var sel_subtracao( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( INT, INT ): return get<INT>( a.v ) - get<INT>( b.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( a.v ) - get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) - get<DOUBLE>( b.v );
                
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) - get<INT>( b.v );
                case tipo( INT, CHAR ): return get<INT>( a.v ) - get<CHAR>( b.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) - get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) - get<DOUBLE>( b.v );
      
                default:
                    return Var();
            }
        }

        static Var sel_multiplicacao( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( INT, INT ): return get<INT>( a.v ) * get<INT>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) * get<DOUBLE>( b.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) * get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) * get<DOUBLE>( b.v );
      
                default:
                    return Var();
            }
        }

        static Var sel_divisao( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( INT, INT ): return get<INT>( a.v ) / get<INT>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) / get<DOUBLE>( b.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) / get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) / get<DOUBLE>( b.v );
      
                default:
                    return Var();
            }
        }

        static Var sel_modulo( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( INT, INT ): return get<INT>( a.v ) % get<INT>( b.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( a.v ) % get<CHAR>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) % get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) % get<INT>( b.v );
      
                default:
                    return Var();
            }
        }

        static Var sel_maior( const Var& a, const Var& b )
        {
            string temp_1, temp_2;
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( BOOL, BOOL ): return get<BOOL>( a.v ) > get<BOOL>( b.v );
                case tipo( INT, INT ): return get<INT>( a.v ) > get<INT>( b.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( a.v ) > get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) > get<DOUBLE>( b.v );
                case tipo( STRING, STRING ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) > get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) > get<DOUBLE>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) > get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) > get<INT>( b.v );

                case tipo( STRING, CHAR ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = string("") + get<CHAR>( b.v );
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = string("") + get<CHAR>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;
      
                default:
                    return Var();
            }

            return temp_1.compare(temp_2) > 0 ? true : false;
        }

        static Var sel_maior_igual( const Var& a, const Var& b )
        {
            string temp_1, temp_2;
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( BOOL, BOOL ): return get<BOOL>( a.v ) >= get<BOOL>( b.v );
                case tipo( INT, INT ): return get<INT>( a.v ) >= get<INT>( b.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( a.v ) >= get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) >= get<DOUBLE>( b.v );
                case tipo( STRING, STRING ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) >= get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) >= get<DOUBLE>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) >= get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) >= get<INT>( b.v );

                case tipo( STRING, CHAR ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = string("") + get<CHAR>( b.v );
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = string("") + get<CHAR>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;
      
                default:
                    return Var();
            }

            return temp_1.compare(temp_2) >= 0 ? true : false;
        }

        static Var sel_igual( const Var& a, const Var& b )
        {
            string temp_1, temp_2;
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( BOOL, BOOL ): return get<BOOL>( a.v ) == get<BOOL>( b.v );
                case tipo( INT, INT ): return get<INT>( a.v ) == get<INT>( b.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( a.v ) == get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) == get<DOUBLE>( b.v );
                case tipo( STRING, STRING ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) == get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) == get<DOUBLE>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) == get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) == get<INT>( b.v );

                case tipo( STRING, INT ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = to_string(get<INT>( b.v ));
                    break;

                case tipo( INT, STRING ):
                    temp_1 = to_string(get<INT>( a.v ));
                    temp_2 = get<STRING>( b.v );
                    break;

                case tipo( STRING, CHAR ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = string("") + get<CHAR>( b.v );
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = string("") + get<CHAR>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;
      
                default:
                    return Var();
            }

            return temp_1.compare(temp_2) == 0 ? true : false;
        }

        static Var sel_and( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( BOOL, BOOL ): return get<BOOL>( a.v ) && get<BOOL>( b.v );

                default:
                    return Var();
            }
        }

        static Var sel_or( const Var& a, const Var& b )
        {
            switch( tipo( a.v.index(), b.v.index() ) )
            {
                case tipo( BOOL, BOOL ): return get<BOOL>( a.v ) || get<BOOL>( b.v );

                default:
                    return Var();
            }
        }

    private:
        Variant v;
};

// ------------------------------- Operadores Externos -------------------------------

ostream& operator << ( ostream& o, const Var& v )
{
    v.print( o );
    return o;
}

inline Var operator + ( const Var& a, const Var& b )
{
    return Var::sel_soma( a, b );
}

inline Var operator - ( const Var& a, const Var& b )
{
    return Var::sel_subtracao( a, b );
}

inline Var operator * ( const Var& a, const Var& b )
{
    return Var::sel_multiplicacao( a, b );
}

inline Var operator / ( const Var& a, const Var& b )
{
    return Var::sel_divisao( a, b );
}

inline Var operator % ( const Var& a, const Var& b )
{
    return Var::sel_modulo( a, b );
}

inline Var operator ! ( const Var::DuplaExclamacao& a )
{
    return a.obterResultado();
}

inline Var operator > ( const Var& a, const Var& b )
{
    return Var::sel_maior( a, b );
}

inline Var operator >= ( const Var& a, const Var& b )
{
    return Var::sel_maior_igual( a, b );
}

inline Var operator < ( const Var& a, const Var& b )
{
    return Var::sel_maior( b, a );
}

inline Var operator <= ( const Var& a, const Var& b )
{
    return Var::sel_maior_igual( b, a );
}

inline Var operator == ( const Var& a, const Var& b )
{
    return Var::sel_igual( a, b );
}

inline Var operator && ( const Var& a, const Var& b )
{
    return Var::sel_and( a, b );
}

inline Var operator || ( const Var& a, const Var& b )
{
    return Var::sel_or( a, b );
}

// ------------------------------- Funções -------------------------------

Var::Object* newObject()
{
    return new Var::Object();
}

Var::Array* newArray()
{
    return new Var::Array();
}

Var print( Var a )
{
    cout << "{ nome: " << a["nome"] << ", "
    << "idade: " << a["idade"](a) << ", "
    << "nascimento: " << a["nascimento"] << ", "
    << "print: " << a["print"] << "}" << endl;

    return Var();
}

void imprime( Var o )
{
    o["print"](o);
}