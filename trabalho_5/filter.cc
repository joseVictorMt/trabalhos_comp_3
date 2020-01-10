#include <algorithm>
#include <type_traits>

using namespace std;

template <
  typename K,
  typename T,
  typename C,
  typename A,
  template <typename, typename, typename, typename> class Structure = map,
  typename Function
>
auto operator | ( const Structure<K, T, C, A> &maple, Function func ) {
  if constexpr ( is_same_v<invoke_result_t<decltype(func), decltype(*maple.begin())>, bool>) {
    vector< pair<K, T> > temp;

    for_each(
      begin( maple ),
      end( maple ),
      [func, &temp]( auto x ) mutable { if( func(x) ){ temp.push_back(x); } }
    );

    return temp;
  } else if constexpr ( is_same_v< invoke_result_t<decltype(func), decltype(*maple.begin())>, pair<string, double> > ) {
    vector< pair<string, double> > temp;
    
    for_each(
      begin( maple ),
      end( maple ),
      [func, &temp]( auto x ) mutable { temp.push_back( func(x) ); }
    );

    return temp;

  }
}

template <
  typename T,
  typename A,
  template <typename, typename> class Structure = vector,
  typename Function
>
auto operator | ( const Structure<T, A> &vect, Function func ) {

  if constexpr ( is_same_v< invoke_result_t<decltype(func), decltype(*vect.begin())>, pair<string, double> > ) {
    vector< pair<string, double> > temp;
    
    for_each(
      begin( vect ),
      end( vect ),
      [func, &temp]( auto x ) mutable { temp.push_back( func(x) ); }
    );

    return temp;

  } else if constexpr ( is_same_v<invoke_result_t<decltype(func), decltype(*vect.begin())>, bool>) {
    vector<T> temp;

    for_each(
      begin( vect ),
      end( vect ),
      [func, &temp]( auto x ) mutable { if( func(x) ) { temp.push_back(x); } }
    );

    return temp;
  } else if constexpr ( is_same_v<invoke_result_t<decltype(func), decltype(*vect.begin())>, string> ) {
    vector<string> temp;

    for_each(
      begin( vect ),
      end( vect ),
      [func, &temp]( auto x ) mutable { temp.push_back(func(x)); }
    );

    return temp;
  }

  for_each( begin( vect ), end( vect ), func );
}

auto operator | ( const vector<string> &vect, decltype(&string::length) func ) {
  vector<int> temp;

  for_each(
    begin( vect ),
    end( vect ),
    [func, &temp]( auto x ) mutable { temp.push_back(x.length()); }
  );

  return temp;
}

template <
  typename T,
  int N,
  typename Function
>
auto operator | ( const T(& array)[N], Function func ) {
  vector<T> temp;
  
  for_each( 
    begin( array ),
    end( array ),
    [func, &temp]( auto x ) mutable { if( func(x) ){ temp.push_back(x); } }
  );

  return temp;
}