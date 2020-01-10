#include <functional>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <cctype>
#include <sstream>
#include <type_traits>

using namespace std;

bool temPrecedencia( string expr ) {
  return (expr.find('+') != string::npos || expr.find('-') != string::npos ) ? true: false;
}

bool ehExpressao( string expr ) {
  return (  (expr.find('+') != string::npos ||
            expr.find('-') != string::npos ||
            expr.find('*') != string::npos ||
            expr.find('/') != string::npos) ) ? true : false;
}

bool exprUnica( string expr ) {
  bool exprParenteses = false;
  int initPar = 0, countPar = 0;
  if(expr[0] == '-' || expr.find("exp") == 0 || expr.find("log") == 0) {
    for( int i = 0; i < expr.size(); i++ ) {
      if(expr[i] == '(' && !exprParenteses) {
        initPar ++; exprParenteses = true;
      } else if(expr[i] == '(' && initPar && exprParenteses) {
        countPar++;
      }else if(expr[i] == ')' && countPar && exprParenteses) {
        countPar--;
      }else if(expr[i] == ')' && !countPar && exprParenteses) {
        initPar--;
      }else if(expr[i] == '(' && !initPar && exprParenteses) {
        if(expr[i-1] == '*' && expr[i-2] == ')')
          initPar++;
        else
          return false;
      }
    }
    return true;
  }
  return false;
}

bool denomIsolado( string expr ) {
  if( expr.find('/') != string::npos ) {
    if( expr[0] == 'x' && expr[1] == '/')
      return true;
    else if( isdigit(expr[0]) ){
      for( int i = 1; i < expr.size(); i++ ) {
        if(isdigit(expr[i]) || expr[i] == '.'){
          continue;
        }if(expr[i] == '/')
          return true;
        else
          return false;
      }
    }
  }
  return false;
}

bool denoExpoenteFora( string expr ) {
  auto exp_opr_i = expr.find('^');
  if(exp_opr_i == string::npos)
    return false;
  else {
    for(auto i = expr.rbegin(); i < expr.rend(); ++i) {
      if(isdigit(*i) || *i == '.' || *i == 'x' || *i == '^')
        continue;
      else if(*i == ')')
        return true;
      else
        return false;
    }
    return true;
  }
}

string elaboraExpr( const string exp1, const char opr, const string exp2 ) {
  string retorno = "";
  switch(opr) {
    case '+':
      if( exp1 != "0" )
        retorno += exp1;
      if( exp2 != "0" ) {
        if( retorno != "" )
          retorno += '+' + exp2;
        else
          retorno += exp2;
      }
      break;
    case '-':
      if( exp1 != "0" && exp1 != "" )
        retorno += exp1;
      if( exp2 != "0" && exp2 != "") {
        retorno += (temPrecedencia(exp2)) && exp2.find('*') == string::npos ? "-(" + exp2 + ')' : '-' + exp2;
      }
      break;
    case '*':
      if( exp1 == "0" || exp2 =="0" ) { break; }
      if( exp1 == "1" && exp2 == "1" ) { retorno += "1"; break; }
      if( exp1 != "1" )
        retorno += (
          !denomIsolado(exp1) &&
          temPrecedencia(exp1) && 
          exp2 != "1" &&
          !exprUnica(exp1)) ? '('+exp1+')' : exp1;
      if( exp2 != "1" ) {
        if(exp1 != "1")
          retorno += (
            !denomIsolado(exp2) &&
            exp2.find('^') == string::npos &&
            temPrecedencia(exp2) &&
            !exprUnica(exp2) ) ? "*("+ exp2+')' : '*'+exp2;
        else
          retorno += exp2;
      }
      break;
    case '^':
      if( exp1 == "0" )
        retorno += "0";
      else if( exp2 == "0" || (exp1 == "1" && exp2 == "1") )
        retorno += "1";
      else {
        retorno += ehExpressao(exp1) && !exprUnica(exp1) ? '('+exp1+')' : exp1;
        if( exp2 != "1" )
          retorno += ehExpressao(exp2) ? "^("+ exp2+')' : '^'+exp2;
      }    
      break;
    case '/':
      if( exp2 == "0" ) { return "Erro. Divisão por zero."; }
      if( exp1 == "0" ) { retorno += "0"; break; }
      retorno += exp1;
      retorno += ehExpressao(exp2) && !denoExpoenteFora(exp2) ? "/("+ exp2+')' : '/'+exp2;
      break;
    default:
      return "Operando desconhecido (bug)";
  }
  return retorno;
}

string convNum( double v ) {
  double initpart;
  // Verifica se o número tem casa decimal maior que 0.
  auto decm = modf(v, &initpart);
  switch( decm == 0 ) {
    case true:
      return to_string((int) initpart);
    default:
      ostringstream convStream;
      convStream << v << setprecision(2);
      return convStream.str();
  }
};

template <typename E, typename Dx>
struct Par {
  const E str;
  const Dx dx_str;

  constexpr Par( E e, Dx dx ) : str(e), dx_str(dx) {}
};

constexpr inline auto x = Par { []() ->string { return "x"; }, []() ->string { return "1"; } }; 

template <typename A, typename B>
constexpr inline auto trata_cte( Par<A,B> opr ) {
  return opr;
}

template <typename C>
constexpr inline auto trata_cte( C cte ) {
  string valor = convNum(cte);
  return Par { [valor]() ->string { return valor; }, []() ->string { return "0"; } };
}

inline auto operator + ( auto f, auto g ) {
  auto fc = trata_cte( f );
  auto gc = trata_cte( g );
  auto expr = elaboraExpr( fc.str(), '+', gc.str() );
  auto deriv = elaboraExpr( fc.dx_str(), '+', gc.dx_str() );
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

inline auto operator - ( auto f, auto g ) {
  auto fc = trata_cte( f );
  auto gc = trata_cte( g );
  auto expr = elaboraExpr( fc.str(), '-', gc.str() );
  auto deriv = elaboraExpr( fc.dx_str(), '-', gc.dx_str() );
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

inline auto operator * ( auto f, auto g ) {
  auto fc = trata_cte( f );
  auto gc = trata_cte( g );
  auto expr = elaboraExpr( fc.str(), '*', gc.str() );
  auto deriv = elaboraExpr(
    elaboraExpr(fc.dx_str(), '*', gc.str()), '+', elaboraExpr(fc.str(), '*', gc.dx_str())
  );
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

constexpr inline auto operator / ( auto f, auto g ) {
  auto fc = trata_cte( f );
  auto gc = trata_cte( g );
  auto expr = elaboraExpr( fc.str(), '/', gc.str() );
  auto deriv = elaboraExpr(
    elaboraExpr(elaboraExpr(fc.dx_str(), '*', gc.str()), '-', elaboraExpr(fc.str(), '*', gc.dx_str())),
    '/', elaboraExpr(gc.str(), '^', "2")
  );
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

inline auto operator ->* ( auto f, auto g ) {
  static_assert( !is_same_v< double, decltype(g) >, "Operador de potenciação definido apenas para inteiros" );
  auto fc = trata_cte( f );
  auto gc = trata_cte( g );
  auto decr_gc = trata_cte( g-1 );
  auto expr = elaboraExpr(fc.str(), '^', gc.str());
  auto deriv= elaboraExpr( gc.str(), '*', fc.dx_str() ) + '*' + elaboraExpr( fc.str(), '^', decr_gc.str() );
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

constexpr inline auto sin( auto f ) {
  auto fc = trata_cte( f );
  auto expr = "sin(" + fc.str() + ')';
  auto deriv = elaboraExpr("cos(" + fc.str() + ")", '*', fc.dx_str());
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

constexpr inline auto cos( auto f ) {
  auto fc = trata_cte( f );
  auto expr = "cos(" + fc.str() + ")";
  auto deriv = elaboraExpr("-sin(" + fc.str() + ")", '*', fc.dx_str());
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

constexpr inline auto exp( auto f ) {
  auto fc = trata_cte( f );
  auto expr = "exp("+fc.str()+')';
  auto deriv = elaboraExpr("exp("+fc.str()+")", '*', fc.dx_str());
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}

constexpr inline auto log( auto f ) {
  auto fc = trata_cte( f );
  auto expr = "log("+fc.str()+')';
  auto deriv = elaboraExpr(elaboraExpr("1", '/', fc.str()), '*', fc.dx_str());
  return Par { [expr](){ return expr; }, [deriv](){ return deriv; } };
}