#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

using namespace std;

class Var {
    public:
        class Erro: public exception
        {
            public:
                Erro( string msg ): msg( msg ) {}

                string operator()() { return msg; }

                string what() { return msg; }
            
            private:
                string msg;
        };

    public:
        class Undefined {};

        class Object {
            public:
                virtual ~Object() {}
        
                virtual void print( ostream& o ) const { 
                    o << "object";
                }
        
                virtual Var func( const Var& arg ) const
                { 
                    throw Erro( "Essa variável não pode ser usada como função" ); 
                }  
        
                Var& lvalue( const string& st ) { return atr[st]; }
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
    
    private:
        template <class F>
        class Function: public Object
        {
            public:
                Function( F f ): f(f) {}

                void print( ostream& o ) const {
                    o << "function"; 
                }
            
                Var func( const Var& arg ) const {
                    return invoke( f, arg );
                }  
        
            private:
                F f;
        };

    private:
        enum { UNDEFINED, BOOL, CHAR, INT, DOUBLE, STRING, OBJECT };

        typedef variant<Undefined,bool,char,int,double,string,shared_ptr<Object>> Variant;
    
    public:
  
        Var(): v() {}
        Var( const char* st ): v( string(st) ) {}
        Var( bool v ): v(v) {}
        Var( char v ): v(v) {}
        Var( int v ): v(v) {}
        Var( double v ): v(v) {}
        Var( string v ): v(v) {}
        Var( Object* v ): v( shared_ptr<Object>( v ) ) {}

        template <typename T>
        Var( T func ): v( shared_ptr<Object>( new Function( func ) ) ) {}

        Var operator()( Var arg ) const
        {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não pode ser usada como função" );

            return get<OBJECT>( v )->func( arg );
        }

        Var& operator[]( const string& st ) {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é objeto" );

            return get<OBJECT>( v )->lvalue( st );
        }

        Var operator[]( const string& st ) const {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é objeto" );

            return get<OBJECT>( v )->rvalue( st );
        }
  
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
                    temp_2 = to_string(get<CHAR>( b.v ));
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = to_string(get<CHAR>( a.v ));
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
                case tipo( CHAR, CHAR ): get<CHAR>( a.v ) >= get<CHAR>( b.v );
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
                    temp_2 = to_string(get<CHAR>( b.v ));
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = to_string(get<CHAR>( a.v ));
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
                case tipo( CHAR, CHAR ): get<CHAR>( a.v ) == get<CHAR>( b.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( a.v ) == get<DOUBLE>( b.v );
                case tipo( STRING, STRING ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = get<STRING>( b.v );
                    break;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( a.v ) == get<INT>( b.v );
                case tipo( INT, DOUBLE ): return get<INT>( a.v ) == get<DOUBLE>( b.v );

                case tipo( INT, CHAR ): return get<INT>( a.v ) == get<CHAR>( b.v );
                case tipo( CHAR, INT ): return get<CHAR>( a.v ) == get<INT>( b.v );

                case tipo( STRING, CHAR ):
                    temp_1 = get<STRING>( a.v );
                    temp_2 = to_string(get<CHAR>( b.v ));
                    break;

                case tipo( CHAR, STRING ):
                    temp_1 = to_string(get<CHAR>( a.v ));
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

        static Var sel_not( const Var& a )
        {
            switch( a.v.index() )
            {
                case BOOL: return !get<BOOL>( a.v );

                default:
                    return Var();
            }
        }
  
    private:
        Variant v;
};

ostream& operator << ( ostream& o, const Var& v ) {
    v.print( o );
    return o;
}

inline Var operator + ( const Var& a, const Var& b ) {
    return Var::sel_soma( a, b );
}

inline Var operator - ( const Var& a, const Var& b ) {
    return Var::sel_subtracao( a, b );
}

inline Var operator * ( const Var& a, const Var& b ) {
    return Var::sel_multiplicacao( a, b );
}

inline Var operator / ( const Var& a, const Var& b ) {
    return Var::sel_divisao( a, b );
}

inline Var operator ! ( const Var& a ) {
    return Var::sel_not( a );
}

inline Var operator > ( const Var& a, const Var& b ) {
    return Var::sel_maior( a, b );
}

inline Var operator >= ( const Var& a, const Var& b ) {
    return Var::sel_maior_igual( a, b );
}

inline Var operator < ( const Var& a, const Var& b ) {
    return !Var::sel_maior_igual( a, b );
}

inline Var operator <= ( const Var& a, const Var& b ) {
    return !Var::sel_maior( a, b );
}

inline Var operator == ( const Var& a, const Var& b ) {
    return Var::sel_igual( a, b );
}

inline Var operator && ( const Var& a, const Var& b ) {
    return Var::sel_and( a, b );
}

inline Var operator || ( const Var& a, const Var& b ) {
    return Var::sel_or( a, b );
}

Var::Object* newObject() {
    return new Var::Object();
}

Var print( Var a ) {
    cout <<
    "{ nome: " << a["nome"] << ", " <<
    "idade: " << a["idade"](a) << ", " <<
    "nascimento: " << a["nascimento"] << ", " <<
    "print: " << a["print"] <<
    "}" << endl;

    return Var();
}

void imprime( Var o )
{
    o["print"](o);
}